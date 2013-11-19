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
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  exclude-result-prefixes="table">
  
  
  <!-- Variable with Merge Style-->
  
  <xsl:template name="WriteMergeStyle">
		<!--Vijayeta,SP2,@table:number-rows-repeated-->
		<!--<xsl:variable name="rowNumber">
          <xsl:choose>
            <xsl:when test="descendant::table:table-row[1]/@table:number-rows-repeated">
              <xsl:value-of select="descendant::table:table-row[1]/@table:number-rows-repeated"/>
            </xsl:when>
            <xsl:otherwise>1</xsl:otherwise>
          </xsl:choose>  
        </xsl:variable>-->
        <xsl:variable name="rowNumber">
          <xsl:choose>
            <xsl:when test="descendant::table:table-row[1]/@table:number-rows-repeated">
					<xsl:choose >
						<xsl:when test ="descendant::table:table-row[1]/@table:number-rows-repeated &gt; 65536">
							<xsl:value-of select ="65536 - (1048576 - descendant::table:table-row[1]/@table:number-rows-repeated)"/>
						</xsl:when>
						<xsl:when test ="descendant::table:table-row[1]/@table:number-rows-repeated &lt;= 65536">
              <xsl:value-of select="descendant::table:table-row[1]/@table:number-rows-repeated"/>
            </xsl:when>
					</xsl:choose>
				</xsl:when>
            <xsl:otherwise>1</xsl:otherwise>
          </xsl:choose>  
        </xsl:variable>
		<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
        <xsl:apply-templates select="descendant::table:table-row[1]" mode="mergestyle">
          <xsl:with-param name="rowNumber">
            <xsl:value-of select="$rowNumber"/>
          </xsl:with-param>
        </xsl:apply-templates>
    
  </xsl:template>
  
  <!-- Check if "Merge Cell" exist in row-->
  <xsl:template match="table:table-row" mode="mergestyle">
    <xsl:param name="rowNumber"/>
    
    <xsl:if test="table:table-cell[@table:number-columns-spanned]">   
      
      <xsl:choose>
        <xsl:when test="node()[1][name()='table:covered-table-cell']">          
          <xsl:for-each select="table:covered-table-cell[1]">
            <xsl:call-template name="MergeRowStyle">
              <xsl:with-param name="rowNumber">
                <xsl:value-of select="$rowNumber"/>
              </xsl:with-param>
              <xsl:with-param name="colNumber">1</xsl:with-param>
            </xsl:call-template>
          </xsl:for-each>            
        </xsl:when>          
        <xsl:otherwise>
          <xsl:for-each select="table:table-cell[1]">
            <xsl:call-template name="MergeRowStyle">
              <xsl:with-param name="rowNumber">
                <xsl:value-of select="$rowNumber"/>
              </xsl:with-param>
              <xsl:with-param name="colNumber">1</xsl:with-param>
            </xsl:call-template>
          </xsl:for-each>
        </xsl:otherwise>
      </xsl:choose>
      
    </xsl:if>
    
    <xsl:variable name="tableId">
      <xsl:value-of select="generate-id(ancestor::table:table)"/>
    </xsl:variable>
    
    <xsl:choose>
      <!-- next row is a sibling -->
		<!-- Bug      :1803593,file, ilas_EVE_Database_V0.02.xlsx and ilas_EVE_Database_V0.03.xlsx
		     Fixed By :Sateesh
			 Desc     :OR condition replaced by AND, which brings down the time to a few swcondsLast
		    <xsl:when test="following::table:table-row[generate-id(ancestor::table:table) = $tableId] and not(following-sibling::node()[1][name() = 'table:table-header-rows' ] or following-sibling::node()[1][name() = 'table:table-row-group' ])">-->
		<xsl:when test="following::table:table-row[generate-id(ancestor::table:table) = $tableId] and not(following-sibling::node()[1][name() = 'table:table-header-rows' ] and following-sibling::node()[1][name() = 'table:table-row-group' ])">
        <xsl:apply-templates select="following::table:table-row[generate-id(ancestor::table:table) = $tableId][1]" mode="mergestyle">
					<!--Vijayeta,SP2,@table:number-rows-repeated-->
					<!--<xsl:with-param name="rowNumber">
            <xsl:choose>
              <xsl:when test="following-sibling::table:table-row[1]/@table:number-rows-repeated">
                <xsl:value-of
                  select="$rowNumber + following-sibling::table:table-row[1]/@table:number-rows-repeated"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$rowNumber + 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>-->
					<xsl:with-param name="rowNumber">
						<xsl:choose>
							<xsl:when test="following-sibling::table:table-row[1]/@table:number-rows-repeated">
								<xsl:choose >
									<xsl:when test ="following-sibling::table:table-row[1]/@table:number-rows-repeated &gt; 65536">
										<xsl:value-of select ="$rowNumber + 65536 - (1048576 - following-sibling::table:table-row[1]/@table:number-rows-repeated)"/>
									</xsl:when>
									<xsl:when test ="following-sibling::table:table-row[1]/@table:number-rows-repeated &lt;= 65536">
										<xsl:value-of select="$rowNumber + following-sibling::table:table-row[1]/@table:number-rows-repeated" />
									</xsl:when>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$rowNumber + 1"/>
							</xsl:otherwise>
						</xsl:choose>
          </xsl:with-param>
					<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
        </xsl:apply-templates>
      </xsl:when>
      <!-- next row is inside header rows -->
      <xsl:when test="following-sibling::node()[1][name() = 'table:table-header-rows' ] or following-sibling::node()[1][name() = 'table:table-row-group' ]">
        <xsl:apply-templates select="following-sibling::table:table-header-rows/table:table-row[1]|following-sibling::table:table-row-group/table:table-row[1]"
          mode="mergestyle">
					<!--Vijayeta,SP2,@table:number-rows-repeated-->
					<!--<xsl:with-param name="rowNumber">
            <xsl:choose>
              <xsl:when
                test="following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated">
                <xsl:value-of
                  select="$rowNumber + following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated"
                />
              </xsl:when>
              <xsl:when
                test="following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated">
                <xsl:value-of
                  select="$rowNumber + following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated"
                />
                </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$rowNumber + 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>-->
					<xsl:with-param name="rowNumber">
						<xsl:choose>
							<xsl:when
							  test="following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated">
								<xsl:choose >
									<xsl:when test ="following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated &gt; 65536">
										<xsl:value-of select ="$rowNumber + 65536 - (1048576 - following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated)"/>
									</xsl:when>
									<xsl:when test ="following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated &lt;= 65536">
										<xsl:value-of select="$rowNumber + following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated"/>
									</xsl:when>
								</xsl:choose>
							</xsl:when>
							<xsl:when
							  test="following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated">
								<xsl:choose >
									<xsl:when test ="following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated &gt; 65536">
										<xsl:value-of select ="$rowNumber + 65536 - (1048576 - following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated)"/>
									</xsl:when>
									<xsl:when test ="following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated &lt;= 65536">
										<xsl:value-of select="$rowNumber + following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated" />
									</xsl:when>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$rowNumber + 1"/>
							</xsl:otherwise>
						</xsl:choose>
          </xsl:with-param>
					<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
        </xsl:apply-templates>
      </xsl:when>
      <!-- this is last row inside header rows, next row is outside -->
      <xsl:when
        test="(parent::node()[name()='table:table-header-rows'] or parent::node()[name()='table:table-row-group']) and not(following-sibling::node()[1][name() = 'table:table-row' ])">
        <xsl:apply-templates select="parent::node()/following-sibling::table:table-row[1]"
          mode="mergestyle">
					<!--Vijayeta,SP2,@table:number-rows-repeated-->
					<!--<xsl:with-param name="rowNumber">
            <xsl:choose>
              <xsl:when
                test="parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated">
                <xsl:value-of
                  select="$rowNumber + parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$rowNumber + 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>-->
					<xsl:with-param name="rowNumber">
						<xsl:choose>
							<xsl:when
							  test="parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated">
								<xsl:choose >
									<xsl:when test ="parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated &gt; 65536">
										<xsl:value-of select ="$rowNumber + 65536 - (1048576 - parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated)"/>
									</xsl:when>
									<xsl:when test ="parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated &lt;= 65536">
										<xsl:value-of select="$rowNumber + parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated"/>
									</xsl:when>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$rowNumber + 1"/>
							</xsl:otherwise>
						</xsl:choose>
          </xsl:with-param>
					<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
        </xsl:apply-templates>
      </xsl:when>
    </xsl:choose>
  </xsl:template>
  
  <!-- to insert merge cell style -->
  <xsl:template name="MergeRowStyle">
    <xsl:param name="rowNumber"/>
    <xsl:param name="colNumber"/>
    
    
    <!-- Merge Cell-->    
    <xsl:choose>
      <xsl:when test="@table:number-columns-spanned">
				<!--Vijayeta,SP2,@table:number-columns-spanned/@table:number-rows-spanned-->
				<xsl:variable name ="tableNumberColumnsSpanned">
					<xsl:choose >
						<xsl:when test ="@table:number-columns-spanned &gt; 256">
							<xsl:value-of select ="256 - (16384 - @table:number-columns-spanned)"/>
						</xsl:when>
						<xsl:when test ="@table:number-columns-spanned &lt;= 256">
							<xsl:value-of select="@table:number-columns-spanned"/>
						</xsl:when>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name ="tableNumberRowsSpanned">
					<xsl:choose >
						<xsl:when test ="@table:number-rows-spanned &gt; 65536">
							<xsl:value-of select ="65536 - (1048576 - @table:number-rows-spanned)"/>
						</xsl:when>
						<xsl:when test ="@table:number-rows-spanned &lt;= 65536">
							<xsl:value-of select="@table:number-rows-spanned"/>
						</xsl:when>
					</xsl:choose>
				</xsl:variable>
        
        <xsl:call-template name="InsertMergeStyle">
          <xsl:with-param name="rowNum">
            <xsl:value-of select="$rowNumber"/>
          </xsl:with-param>
          <xsl:with-param name="colNum">
            <xsl:value-of select="$colNumber - 1"/>
          </xsl:with-param>
					<!--<xsl:with-param name="rowNumEnd">
            <xsl:value-of select="$rowNumber + @table:number-rows-spanned - 1"/>
          </xsl:with-param>
          <xsl:with-param name="colNumEnd">
            <xsl:value-of select="$colNumber + @table:number-columns-spanned - 2"/>
					</xsl:with-param>-->
					<xsl:with-param name="rowNumEnd">
						<xsl:value-of select="$rowNumber + $tableNumberRowsSpanned - 1"/>
					</xsl:with-param>
					<xsl:with-param name="colNumEnd">
						<xsl:value-of select="$colNumber + $tableNumberColumnsSpanned - 2"/>
          </xsl:with-param>
          <xsl:with-param name="StyleName">
            <xsl:value-of select="@table:style-name"/>
          </xsl:with-param>
        </xsl:call-template>
				<!--Vijayeta,SP2,@table:number-columns-spanned/@table:number-rows-spanned,End-->
        
        
      </xsl:when>
      <xsl:otherwise/>
    </xsl:choose>
    
    <!-- Check if next cell exist -->
    
    <xsl:if test="following-sibling::table:table-cell">
			<!--Vijayeta,SP2,@table:number-columns-repeated-->
			<!--<xsl:variable name="ColumnsRepeated">
        <xsl:choose>
          <xsl:when test="@table:number-columns-repeated">
            <xsl:value-of select="$colNumber + @table:number-columns-repeated"/>
          </xsl:when>        
          <xsl:otherwise>
            <xsl:value-of select="$colNumber + 1"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>-->
      <xsl:variable name="ColumnsRepeated">
        <!--<xsl:choose>
          <xsl:when test="@table:number-columns-repeated">
						<xsl:choose >
							<xsl:when test ="@table:number-columns-repeated &gt; 256">
								<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
							</xsl:when>
							<xsl:when test ="@table:number-columns-repeated &lt;= 256">
            <xsl:value-of select="$colNumber + @table:number-columns-repeated"/>
          </xsl:when>        
						</xsl:choose>
					</xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$colNumber + 1"/>
          </xsl:otherwise>
        </xsl:choose>-->
		  <xsl:variable name ="tempOfficeVersion">
			  <xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
		  </xsl:variable>
		  <xsl:choose>
			  <xsl:when test="@table:number-columns-repeated">
				  <xsl:choose>
					  <xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
						  <xsl:value-of select="$colNumber + @table:number-columns-repeated"/>
					  </xsl:when>
					  <xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
						  <xsl:choose>
							  <xsl:when test ="@table:number-columns-repeated &gt; 256">
								  <xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
							  </xsl:when>
							  <xsl:otherwise>
								  <xsl:value-of select="$colNumber + @table:number-columns-repeated"/>
							  </xsl:otherwise>
						  </xsl:choose>
					  </xsl:when>
					  <xsl:otherwise>
						  <xsl:value-of select="$colNumber + @table:number-columns-repeated"/>
					  </xsl:otherwise>
				  </xsl:choose>
			  </xsl:when>
			  <xsl:otherwise>
				  <xsl:value-of select="$colNumber + 1"/>
			  </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
			<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
      <xsl:choose>
        <xsl:when test="following-sibling::node()[1][name()='table:covered-table-cell']">
          <xsl:for-each select="following-sibling::table:covered-table-cell[1]">
            <xsl:call-template name="MergeRowStyle">
              <xsl:with-param name="rowNumber">
                <xsl:value-of select="$rowNumber"/>
              </xsl:with-param>
              <xsl:with-param name="colNumber">
                <xsl:value-of select="$ColumnsRepeated"/>            
              </xsl:with-param>
            </xsl:call-template>
          </xsl:for-each>    
        </xsl:when>
        
        <xsl:otherwise>
          <xsl:for-each select="following-sibling::table:table-cell[1]">
            <xsl:call-template name="MergeRowStyle">
              <xsl:with-param name="rowNumber">
                <xsl:value-of select="$rowNumber"/>
              </xsl:with-param>
              <xsl:with-param name="colNumber">
                <xsl:value-of select="$ColumnsRepeated"/>            
              </xsl:with-param>
            </xsl:call-template>
          </xsl:for-each>
        </xsl:otherwise>
      </xsl:choose>   
      
    </xsl:if>
    
  </xsl:template>
  
  
  <xsl:template name="InsertMergeStyle">
    <xsl:param name="rowNum"/>
    <xsl:param name="colNum"/>
    <xsl:param name="rowNumEnd"/>
    <xsl:param name="colNumEnd"/>
    <xsl:param name="StyleName"/>
    <xsl:param name="Result"/>
      
    
     <xsl:variable name="InsertMergeStyleCol">
       <xsl:call-template name="InsertMergeStyleCol">
       <xsl:with-param name="rowNum">
         <xsl:value-of select="$rowNum"/>
       </xsl:with-param>
       <xsl:with-param name="colNum">
         <xsl:value-of select="$colNum"/>
       </xsl:with-param>
       <xsl:with-param name="colNumEnd">
         <xsl:value-of select="$colNumEnd"/>
       </xsl:with-param>
       <xsl:with-param name="StyleName">
         <xsl:value-of select="$StyleName"/>
       </xsl:with-param>      
       </xsl:call-template>
     </xsl:variable>
      
      <xsl:choose>
        <xsl:when test="$rowNum &lt; $rowNumEnd">
          <xsl:call-template name="InsertMergeStyle">
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum + 1"/>
            </xsl:with-param>
            <xsl:with-param name="colNum">
              <xsl:value-of select="$colNum"/>
            </xsl:with-param>
            <xsl:with-param name="rowNumEnd">
              <xsl:value-of select="$rowNumEnd"/>
            </xsl:with-param>
            <xsl:with-param name="colNumEnd">
              <xsl:value-of select="$colNumEnd"/>
            </xsl:with-param>
            <xsl:with-param name="StyleName">
              <xsl:value-of select="$StyleName"/>
            </xsl:with-param>
            <xsl:with-param name="Result">
              <xsl:value-of select="concat($Result, $InsertMergeStyleCol)"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="concat($Result, $InsertMergeStyleCol)"/>
        </xsl:otherwise>
      </xsl:choose>
      
      
      

  </xsl:template>
  
  <xsl:template name="InsertMergeStyleCol">
    <xsl:param name="rowNum"/>
    <xsl:param name="colNum"/>    
    <xsl:param name="colNumEnd"/>
    <xsl:param name="StyleName"/>
    <xsl:param name="Result"/>
    
      <xsl:variable name="colNumChar">
      <xsl:call-template name="NumbersToChars">
        <xsl:with-param name="num">
          <xsl:value-of select="$colNum"/>
        </xsl:with-param>
      </xsl:call-template>
      </xsl:variable>
      
      <xsl:choose>
        <xsl:when test="$colNum &lt; $colNumEnd">
          <xsl:call-template name="InsertMergeStyleCol">
            <xsl:with-param name="rowNum">
              <xsl:value-of select="$rowNum"/>
            </xsl:with-param>
            <xsl:with-param name="colNum">
              <xsl:value-of select="$colNum + 1"/>
            </xsl:with-param>
            <xsl:with-param name="colNumEnd">
              <xsl:value-of select="$colNumEnd"/>
            </xsl:with-param>
            <xsl:with-param name="StyleName">
              <xsl:value-of select="$StyleName"/>
            </xsl:with-param>
            <xsl:with-param name="Result">
              <xsl:value-of select="concat($Result, $colNumChar, $rowNum, ':', $StyleName, ';')"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="concat($Result, $colNumChar, $rowNum, ':', $StyleName, ';')"/>
        </xsl:otherwise>
      </xsl:choose>
      
      
  </xsl:template>
  
  <!-- Variable with Merge Cell -->
  
  <xsl:template name="WriteMergeCell">
		<!--Vijayeta,SP2,@table:number-rows-repeated-->
		<!--<xsl:variable name="rowNumber">
          <xsl:choose>
            <xsl:when test="descendant::table:table-row[1]/@table:number-rows-repeated">
              <xsl:value-of select="descendant::table:table-row[1]/@table:number-rows-repeated"/>
            </xsl:when>
            <xsl:otherwise>1</xsl:otherwise>
          </xsl:choose>  
        </xsl:variable>-->
        <xsl:variable name="rowNumber">
          <xsl:choose>
            <xsl:when test="descendant::table:table-row[1]/@table:number-rows-repeated">
					<xsl:choose >
						<xsl:when test ="descendant::table:table-row[1]/@table:number-rows-repeated &gt; 65536">
							<xsl:value-of select ="65536 - (1048576 - descendant::table:table-row[1]/@table:number-rows-repeated)"/>
						</xsl:when>
						<xsl:when test ="descendant::table:table-row[1]/@table:number-rows-repeated &lt;= 65536">
              <xsl:value-of select="descendant::table:table-row[1]/@table:number-rows-repeated"/>
            </xsl:when>
					</xsl:choose>
				</xsl:when>
            <xsl:otherwise>1</xsl:otherwise>
          </xsl:choose>  
        </xsl:variable>
		<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
        <xsl:apply-templates select="descendant::table:table-row[1]" mode="merge">
          <xsl:with-param name="rowNumber">
            <xsl:value-of select="$rowNumber"/>
          </xsl:with-param>
        </xsl:apply-templates>

    <xsl:variable name="tableId">
      <xsl:value-of select="generate-id(ancestor::table:table-row-group)"/>
    </xsl:variable>
    
  </xsl:template>
  
  <!-- Insert Merge Cells -->
  
  <xsl:template name="InsertMergeCells">
    <xsl:param name="MergeCell"/>
    <xsl:if test="$MergeCell != ''">
    <mergeCells>
      <xsl:attribute name="count">
        <xsl:value-of select="count(table:table-row/table:table-cell[@table:number-columns-spanned])"/>
      </xsl:attribute>
      <xsl:call-template name="InsertMerge">
        <xsl:with-param name="MergeCell">
          <xsl:value-of select="$MergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </mergeCells>
    </xsl:if>
  </xsl:template>
  
  <!-- Insert Merge Cell -->
  
  <xsl:template name="InsertMerge">
    <xsl:param name="MergeCell"/>
   
      <mergeCell>      
        <xsl:attribute name="ref">
          <xsl:value-of select="substring-before($MergeCell, ';')"/>
        </xsl:attribute>      
      </mergeCell>
    
    <xsl:if test="substring-after($MergeCell, ';') != ''">
      <xsl:call-template name="InsertMerge">
        <xsl:with-param name="MergeCell">
          <xsl:value-of select="substring-after($MergeCell, ';')"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
    
  </xsl:template>
  
  
  <!-- Check if this cell is starting merge cells -->
  <xsl:template name="CheckIfMergeColl">
   <xsl:param name="MergeCell"/>     
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>

    <xsl:choose>
      <xsl:when test="$MergeCell != ''">
        
        <xsl:variable name="Merge">
          <xsl:value-of select="concat(';', $MergeCell)"/>
        </xsl:variable>
        
        <xsl:variable name="CollChar">
          <xsl:call-template name="NumbersToChars">
            <xsl:with-param name="num">
              <xsl:value-of select="$colNumber"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>
        
        <xsl:variable name="Cell">
          <xsl:value-of select="concat(';', concat(concat($CollChar, $rowNumber), ':'))"/>
        </xsl:variable>

        <xsl:choose>
          <xsl:when test="contains($Merge, $Cell)">
            <xsl:text>start</xsl:text>
          </xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
        
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>false</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>
  
  <!-- Check if this cell is in merge cells -->
  <xsl:template name="CheckIfInMerge">
    <xsl:param name="MergeCell"/>     
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    
    <xsl:variable name="ref">
      <xsl:value-of select="substring-before($MergeCell, ';')"/>
    </xsl:variable>
    
    <xsl:variable name="StartMergeCell">
        <xsl:value-of select="substring-before($ref, ':')"/>
    </xsl:variable>
    
    <xsl:variable name="EndMergeCell">
      <xsl:value-of select="substring-after($ref, ':')"/>
    </xsl:variable>
    
    <xsl:variable name="StartColNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$StartMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:variable name="StartRowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$StartMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:variable name="EndColNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$EndMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:variable name="EndRowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$EndMergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    
    <xsl:choose>
      
      <!-- Checks if this cell is in "Merge Cell" -->
      <xsl:when
        test="($colNumber &gt;= $StartColNum and $colNumber &lt;= $EndColNum) and ($rowNumber &gt;= $StartRowNum and $rowNumber &lt;= $EndRowNum)">
        <xsl:value-of select="$StartMergeCell"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="substring-after($MergeCell, ';') != ''">
            <xsl:call-template name="CheckIfInMerge">
              <xsl:with-param name="MergeCell">
                <xsl:value-of select="substring-after($MergeCell, ';')"/>
              </xsl:with-param>
              <xsl:with-param name="colNumber">
                <xsl:value-of select="$colNumber"/>
              </xsl:with-param>
              <xsl:with-param name="rowNumber">
                <xsl:value-of select="$rowNumber"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>false</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  
  <!-- Check if "Merge Cell" exist in row-->
  <xsl:template match="table:table-row" mode="merge">
    <xsl:param name="rowNumber"/>

    <xsl:if test="table:table-cell[@table:number-columns-spanned]">   
      
      <xsl:choose>
        <xsl:when test="node()[1][name()='table:covered-table-cell']">          
          <xsl:for-each select="table:covered-table-cell[1]">
            <xsl:call-template name="MergeRow">
              <xsl:with-param name="rowNumber">
                <xsl:value-of select="$rowNumber"/>
              </xsl:with-param>
              <xsl:with-param name="colNumber">1</xsl:with-param>
            </xsl:call-template>
          </xsl:for-each>            
        </xsl:when>          
        <xsl:otherwise>
          <xsl:for-each select="table:table-cell[1]">
            <xsl:call-template name="MergeRow">
              <xsl:with-param name="rowNumber">
                <xsl:value-of select="$rowNumber"/>
              </xsl:with-param>
              <xsl:with-param name="colNumber">1</xsl:with-param>
            </xsl:call-template>
          </xsl:for-each>
        </xsl:otherwise>
      </xsl:choose>
      
    </xsl:if>

    <xsl:variable name="tableId">
      <xsl:value-of select="generate-id(ancestor::table:table)"/>
    </xsl:variable>

    <!-- Check if next row exist -->
    <xsl:choose>
      <!-- next row is a sibling -->
      <xsl:when test="following-sibling::table:table-row[generate-id(ancestor::table:table) = $tableId] and not(following-sibling::node()[1][name() = 'table:table-header-rows' ] or following-sibling::node()[1][name() = 'table:table-row-group' ])">
        <xsl:apply-templates select="following::table:table-row[1][generate-id(ancestor::table:table) = $tableId][1]" mode="merge">
					<!--Vijayeta,SP2,@table:number-rows-repeated-->
					<!--<xsl:with-param name="rowNumber">
            <xsl:choose>
              <xsl:when test="following-sibling::table:table-row[1]/@table:number-rows-repeated">
                <xsl:value-of select="$rowNumber + following-sibling::table:table-row[1]/@table:number-rows-repeated"/>    
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$rowNumber + 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>-->
          <xsl:with-param name="rowNumber">
            <xsl:choose>
              <xsl:when test="following-sibling::table:table-row[1]/@table:number-rows-repeated">
								<xsl:choose >
									<xsl:when test ="following-sibling::table:table-row[1]/@table:number-rows-repeated &gt; 65536">
										<xsl:value-of select ="$rowNumber + 65536 - (1048576 - following-sibling::table:table-row[1]/@table:number-rows-repeated)"/>
									</xsl:when>
									<xsl:when test ="following-sibling::table:table-row[1]/@table:number-rows-repeated &lt;= 65536">
                <xsl:value-of select="$rowNumber + following-sibling::table:table-row[1]/@table:number-rows-repeated"/>    
              </xsl:when>
								</xsl:choose>
							</xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$rowNumber + 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
					<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
        </xsl:apply-templates>
      </xsl:when>
      <!-- next row is inside header rows or in group-->
      <xsl:when test="following-sibling::node()[1][name() = 'table:table-header-rows' ]">
       <xsl:apply-templates select="following-sibling::table:table-header-rows/table:table-row[1]"
          mode="merge">
					<!--Vijayeta,SP2,@table:number-rows-repeated-->
					<!--<xsl:with-param name="rowNumber">
            <xsl:choose>
              <xsl:when
                test="following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated">
                <xsl:value-of
                  select="$rowNumber + following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated"
                />
              </xsl:when>             
              <xsl:otherwise>
                <xsl:value-of select="$rowNumber + 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>-->
					<xsl:with-param name="rowNumber">
						<xsl:choose>
							<xsl:when test="following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated">
								<xsl:choose >
									<xsl:when test ="following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated &gt; 65536">
										<xsl:value-of select ="$rowNumber + 65536 - (1048576 - following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated)"/>
									</xsl:when>
									<xsl:when test ="following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated &lt;= 65536">
										<xsl:value-of select="$rowNumber + following-sibling::table:table-header-rows/table:table-row[1]/@table:number-rows-repeated" />
									</xsl:when>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$rowNumber + 1"/>
							</xsl:otherwise>
						</xsl:choose>
          </xsl:with-param>
					<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
        </xsl:apply-templates>
      </xsl:when>
      <xsl:when test="following-sibling::node()[1][name() = 'table:table-row-group' ]">
        <xsl:apply-templates select="following-sibling::table:table-row-group[1]/descendant::table:table-row[1]"
          mode="merge">
					<!--Vijayeta,SP2,@table:number-rows-repeated-->
					<!--<xsl:with-param name="rowNumber">
            <xsl:choose>           
              <xsl:when
                test="following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated">
                <xsl:value-of
                  select="$rowNumber + following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$rowNumber + 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>-->
					<xsl:with-param name="rowNumber">
						<xsl:choose>
							<xsl:when test="following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated">
								<xsl:choose >
									<xsl:when test ="following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated &gt; 65536">
										<xsl:value-of select ="$rowNumber + 65536 - (1048576 - following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated)"/>
									</xsl:when>
									<xsl:when test ="following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated &lt;= 65536">
										<xsl:value-of select="$rowNumber + following-sibling::table:table-row-group/table:table-row[1]/@table:number-rows-repeated"/>
									</xsl:when>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$rowNumber + 1"/>
							</xsl:otherwise>
						</xsl:choose>
          </xsl:with-param>
					<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
        </xsl:apply-templates>    
      </xsl:when>
      <!-- this is last row inside header rows or group, next row is outside -->
      <xsl:when test="(parent::node()[name()='table:table-header-rows'] or parent::node()[name()='table:table-row-group']) and not(following-sibling::node()[1][name() = 'table:table-row' ])">
       <xsl:choose>
          <xsl:when test="parent::node()[name()='table:table-header-rows']">
            <xsl:apply-templates select="parent::node()/following-sibling::table:table-row[1]" mode="merge">
							<!--Vijayeta,SP2,@table:number-rows-repeated-->
							<!--<xsl:with-param name="rowNumber">
              <xsl:choose>
              <xsl:when
              test="parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated">
              <xsl:value-of
              select="$rowNumber + parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated"
              />
              </xsl:when>
              <xsl:otherwise>
              <xsl:value-of select="$rowNumber + 1"/>
              </xsl:otherwise>
              </xsl:choose>
							</xsl:with-param>-->
							<xsl:with-param name="rowNumber">
								<xsl:choose>
									<xsl:when test="parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated">
										<xsl:choose >
											<xsl:when test ="parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated &gt; 65536">
												<xsl:value-of select ="$rowNumber + 65536 - (1048576 - parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated)"/>
											</xsl:when>
											<xsl:when test ="parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated &lt;= 65536">
												<xsl:value-of
										select="$rowNumber + parent::node()/following-sibling::table:table-row[1]/@table:number-rows-repeated"
              />
											</xsl:when>
										</xsl:choose>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$rowNumber + 1"/>
									</xsl:otherwise>
								</xsl:choose>
              </xsl:with-param>             
							<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
              </xsl:apply-templates>    
          </xsl:when>
         <xsl:when test="ancestor::table:table-row-group">
          
           <xsl:apply-templates select="parent::node()/following-sibling::table:table-row[1]" mode="merge">
							<!--Vijayeta,SP2,@table:number-rows-repeated-->
							<!--<xsl:with-param name="rowNumber">
              <xsl:choose>
              <xsl:when
                test="ancestor::node()[name()='table:table-row-group']/following-sibling::table:table-row[1]/@table:number-rows-repeated">
              <xsl:value-of
                select="$rowNumber + ancestor::node()[name()='table:table-row-group']/following-sibling::table:table-row[1]/@table:number-rows-repeated"
              />
              </xsl:when>
              <xsl:otherwise>
              <xsl:value-of select="$rowNumber + 1"/>
              </xsl:otherwise>
              </xsl:choose>
							</xsl:with-param>-->
							<xsl:with-param name="rowNumber">
								<xsl:choose>
									<xsl:when test="ancestor::node()[name()='table:table-row-group']/following-sibling::table:table-row[1]/@table:number-rows-repeated">
										<xsl:choose >
											<xsl:when test ="ancestor::node()[name()='table:table-row-group']/following-sibling::table:table-row[1]/@table:number-rows-repeated &gt; 65536">
												<xsl:value-of select ="$rowNumber + 65536 - (1048576 - ancestor::node()[name()='table:table-row-group']/following-sibling::table:table-row[1]/@table:number-rows-repeated)"/>
											</xsl:when>
											<xsl:when test ="ancestor::node()[name()='table:table-row-group']/following-sibling::table:table-row[1]/@table:number-rows-repeated &lt;= 65536">
												<xsl:value-of
										  select="$rowNumber + ancestor::node()[name()='table:table-row-group']/following-sibling::table:table-row[1]/@table:number-rows-repeated"
              />
											</xsl:when>
										</xsl:choose>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$rowNumber + 1"/>
									</xsl:otherwise>
								</xsl:choose>
              </xsl:with-param>          
							<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
           </xsl:apply-templates>           
         </xsl:when>         
        </xsl:choose>        
      </xsl:when>      
    </xsl:choose>
  </xsl:template>
  
  <!-- to insert merge cell -->
  <xsl:template name="MergeRow">
    <xsl:param name="rowNumber"/>
    <xsl:param name="colNumber"/>
    
    
    <!-- Merge Cell-->    
    <xsl:choose>
      <xsl:when test="@table:number-columns-spanned">
				<!--Vijayeta,SP2,@table:number-columns-spanned/@table:number-rows-spanned-->
				<xsl:variable name ="tableNumberColumnsSpanned">
					<xsl:choose >
						<xsl:when test ="@table:number-columns-spanned &gt; 256">
							<xsl:value-of select ="256 - (16384 - @table:number-columns-spanned)"/>
						</xsl:when>
						<xsl:when test ="@table:number-columns-spanned &lt;= 256">
							<xsl:value-of select="@table:number-columns-spanned"/>
						</xsl:when>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name ="tableNumberRowsSpanned">
					<xsl:choose >
						<xsl:when test ="@table:number-rows-spanned &gt; 65536">
							<xsl:value-of select ="65536 - (1048576 - @table:number-rows-spanned)"/>
						</xsl:when>
						<xsl:when test ="@table:number-rows-spanned &lt;= 65536">
							<xsl:value-of select="@table:number-rows-spanned"/>
						</xsl:when>
					</xsl:choose>
				</xsl:variable>
          <xsl:variable name="CollStartChar">
            <xsl:call-template name="NumbersToChars">
              <xsl:with-param name="num">
                <xsl:value-of select="$colNumber - 1"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>
          
				<!--<xsl:variable name="CollEndChar">
            <xsl:call-template name="NumbersToChars">
              <xsl:with-param name="num">
                <xsl:value-of select="@table:number-columns-spanned + $colNumber - 2"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>
         
        <xsl:variable name="rowsSpanned">
          <xsl:choose>
            <xsl:when test="@table:number-rows-spanned">
              <xsl:value-of select="@table:number-rows-spanned"/>
            </xsl:when>
            <xsl:otherwise>1</xsl:otherwise>
          </xsl:choose>          
				</xsl:variable>-->
				<xsl:variable name="CollEndChar">
					<xsl:call-template name="NumbersToChars">
						<xsl:with-param name="num">
							<xsl:value-of select="$tableNumberColumnsSpanned + $colNumber - 2"/>
						</xsl:with-param>
					</xsl:call-template>
        </xsl:variable>
        
				<xsl:variable name="rowsSpanned">
					<xsl:choose>
						<xsl:when test="@table:number-rows-spanned">
							<xsl:value-of select="$tableNumberRowsSpanned"/>
						</xsl:when>
						<xsl:otherwise>1</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<!--Vijayeta,SP2,@table:number-columns-spanned/@table:number-rows-spanned,End-->
        <xsl:value-of select="concat(concat(concat(concat($CollStartChar,$rowNumber),':'), concat($CollEndChar, $rowNumber + $rowsSpanned - 1)), ';')"/>
        
      </xsl:when>
      <xsl:otherwise/>
    </xsl:choose>
    
    <!-- Check if next cell exist -->
    
    <xsl:if test="following-sibling::table:table-cell">
      
			<!--<xsl:variable name="ColumnsRepeated">
        <xsl:choose>
          <xsl:when test="@table:number-columns-repeated">
            <xsl:value-of select="$colNumber + @table:number-columns-repeated"/>
          </xsl:when>        
          <xsl:otherwise>
            <xsl:value-of select="$colNumber + 1"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>-->
			<!--Vijayeta,SP2,@table:number-columns-repeated-->
      <xsl:variable name="ColumnsRepeated">
        <!--<xsl:choose>
          <xsl:when test="@table:number-columns-repeated">
						<xsl:choose >
							<xsl:when test ="@table:number-columns-repeated &gt; 256">
								<xsl:value-of select ="$colNumber + (256 - (16384 - (@table:number-columns-repeated - 1)))"/>
							</xsl:when>
							<xsl:when test ="@table:number-columns-repeated &lt;= 256">
            <xsl:value-of select="$colNumber + @table:number-columns-repeated"/>
          </xsl:when>        
						</xsl:choose>
					</xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$colNumber + 1"/>
          </xsl:otherwise>
        </xsl:choose>-->
		  <xsl:variable name ="tempOfficeVersion">
			  <xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
		  </xsl:variable>
		  <xsl:choose>
			  <xsl:when test="@table:number-columns-repeated">
				  <xsl:choose>
					  <xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
						  <xsl:value-of select="$colNumber + @table:number-columns-repeated"/>
					  </xsl:when>
					  <xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
						  <xsl:choose>
							  <xsl:when test ="@table:number-columns-repeated &gt; 256">
								  <xsl:value-of select ="$colNumber + (256 - (16384 - (@table:number-columns-repeated - 1)))"/>
							  </xsl:when>
							  <xsl:otherwise>
								  <xsl:value-of select="$colNumber + @table:number-columns-repeated"/>
							  </xsl:otherwise>
						  </xsl:choose>
					  </xsl:when>
					  <xsl:otherwise>
						  <xsl:value-of select="$colNumber + @table:number-columns-repeated"/>
					  </xsl:otherwise>
				  </xsl:choose>
			  </xsl:when>
			  <xsl:otherwise>
				  <xsl:value-of select="$colNumber + 1"/>
			  </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
			<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
      
      <xsl:choose>
        <xsl:when test="following-sibling::node()[1][name()='table:covered-table-cell']">
          <xsl:for-each select="following-sibling::table:covered-table-cell[1]">
            <xsl:call-template name="MergeRow">
              <xsl:with-param name="rowNumber">
                <xsl:value-of select="$rowNumber"/>
              </xsl:with-param>
              <xsl:with-param name="colNumber">
                <xsl:value-of select="$ColumnsRepeated"/>            
              </xsl:with-param>
            </xsl:call-template>
          </xsl:for-each>    
        </xsl:when>
        
        <xsl:otherwise>
	    <xsl:for-each select="following-sibling::table:table-cell[1]">
            <xsl:call-template name="MergeRow">
              <xsl:with-param name="rowNumber">
                <xsl:value-of select="$rowNumber"/>
              </xsl:with-param>
              <xsl:with-param name="colNumber">
                <xsl:value-of select="$ColumnsRepeated"/>            
              </xsl:with-param>
            </xsl:call-template>
          </xsl:for-each>
		</xsl:otherwise>
      </xsl:choose>   
      
    </xsl:if>
    
  </xsl:template>
  
</xsl:stylesheet>
