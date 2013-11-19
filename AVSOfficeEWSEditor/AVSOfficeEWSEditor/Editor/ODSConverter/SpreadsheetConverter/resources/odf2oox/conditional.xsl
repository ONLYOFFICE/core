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
  xmlns:config="urn:oasis:names:tc:opendocument:xmlns:config:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
  xmlns:oox="urn:oox"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0" exclude-result-prefixes="table r">


  <!-- search coditional -->
  <xsl:template match="table:table-row" mode="conditional">
    <xsl:param name="rowNumber"/>
    <xsl:param name="cellNumber"/>
    <xsl:param name="TableColumnTagNum"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="tableName"/>

    <xsl:apply-templates
      select="child::node()[name() = 'table:table-cell' or name()= 'table:covered-table-cell'][1]"
      mode="conditional">
      <xsl:with-param name="colNumber">
        <xsl:text>0</xsl:text>
      </xsl:with-param>
      <xsl:with-param name="rowNumber" select="$rowNumber"/>
      <xsl:with-param name="TableColumnTagNum" select="$TableColumnTagNum"/>
      <xsl:with-param name="MergeCell" select="$MergeCell"/>
      <xsl:with-param name="tableName" select="$tableName"/>
		<!--Vijayeta,SP2,@table:number-rows-repeated-->
		<!--<xsl:with-param name="RepetedRow">
        <xsl:if test="@table:number-rows-repeated != ''">
          <xsl:value-of select="@table:number-rows-repeated - 1"/>
        </xsl:if>
      </xsl:with-param>-->
      <xsl:with-param name="RepetedRow">
        <xsl:if test="@table:number-rows-repeated != ''">
				<xsl:choose >
					<xsl:when test ="@table:number-rows-repeated &gt; 65536">
						<xsl:value-of select ="65536 - (1048576 - @table:number-rows-repeated) - 1"/>
					</xsl:when>
					<xsl:when test ="@table:number-rows-repeated &lt;= 65536">
          <xsl:value-of select="@table:number-rows-repeated - 1"/>
					</xsl:when>
				</xsl:choose>
        </xsl:if>
      </xsl:with-param>
    </xsl:apply-templates>
	  <!--Vijayeta,SP2,@table:number-rows-repeated,End-->
    <xsl:variable name="tableId">
      <xsl:value-of select="generate-id(ancestor::table:table)"/>
    </xsl:variable>

    <!-- check next row -->
    <xsl:choose>
      <!-- next row is a sibling -->
      <xsl:when test="following::table:table-row[generate-id(ancestor::table:table) = $tableId]">
        <xsl:apply-templates
          select="following::table:table-row[generate-id(ancestor::table:table) = $tableId][1]"
          mode="conditional">
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
          <xsl:with-param name="cellNumber">
            <xsl:text>0</xsl:text>
          </xsl:with-param>
			<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
          <xsl:with-param name="TableColumnTagNum" select="$TableColumnTagNum"/>
          <xsl:with-param name="MergeCell" select="$MergeCell"/>
          <xsl:with-param name="tableName" select="$tableName"/>
        </xsl:apply-templates>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="table:covered-table-cell" mode="conditional">
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    <xsl:param name="TableColumnTagNum"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="tableName"/>

    <xsl:if test="following-sibling::table:table-cell">
      <xsl:apply-templates
        select="following-sibling::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ][1]"
        mode="conditional">
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
					<!--<xsl:choose>
            <xsl:when test="@table:number-columns-repeated != ''">
					  <xsl:choose >
						  <xsl:when test ="@table:number-columns-repeated &gt; 256">
							  <xsl:value-of select ="number($colNumber) + number(256 - (16384 - number(@table:number-columns-repeated)))"/>
						  </xsl:when>
						  <xsl:when test ="@table:number-columns-repeated &lt;= 256">
							  <xsl:value-of
						  select="number($colNumber) + number(@table:number-columns-repeated)"/>
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
						<xsl:when test="@table:number-columns-repeated != ''">
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
        <xsl:with-param name="TableColumnTagNum" select="$TableColumnTagNum"/>
        <xsl:with-param name="MergeCell" select="$MergeCell"/>
        <xsl:with-param name="tableName" select="$tableName"/>
      </xsl:apply-templates>
    </xsl:if>

  </xsl:template>

  <!-- insert conditional -->
  <xsl:template match="table:table-cell" mode="conditional">
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    <xsl:param name="TableColumnTagNum"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="tableName"/>
    <xsl:param name="RepetedRow"/>
    <xsl:param name="RepetedCol" select="1"/>

    <xsl:call-template name="InsertConditionalCol">
      <xsl:with-param name="colNumber">
        <xsl:value-of select="$colNumber"/>
      </xsl:with-param>
      <xsl:with-param name="rowNumber">
        <xsl:value-of select="$rowNumber"/>
      </xsl:with-param>
      <xsl:with-param name="TableColumnTagNum">
        <xsl:value-of select="$TableColumnTagNum"/>
      </xsl:with-param>
      <xsl:with-param name="MergeCell">
        <xsl:value-of select="$MergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="tableName">
        <xsl:value-of select="$tableName"/>
      </xsl:with-param>
      <xsl:with-param name="RepetedRow">
        <xsl:value-of select="$RepetedRow"/>
      </xsl:with-param>
      <xsl:with-param name="RepetedCol">
        <xsl:value-of select="$RepetedCol"/>
      </xsl:with-param>
    </xsl:call-template>
    
    <xsl:if test="following-sibling::table:table-cell">
      <xsl:apply-templates
        select="following-sibling::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell'][1]"
        mode="conditional">
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
					<!--<xsl:choose>
            <xsl:when test="@table:number-columns-repeated != ''">
					  <xsl:choose >
						  <xsl:when test ="@table:number-columns-repeated &gt; 256">
							  <xsl:value-of select ="number($colNumber) + number(256 - (16384 - number(@table:number-columns-repeated)))"/>
						  </xsl:when>
						  <xsl:when test ="@table:number-columns-repeated &lt;= 256">
							  <xsl:value-of
						  select="number($colNumber) + number(@table:number-columns-repeated)"/>
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
						<xsl:when test="@table:number-columns-repeated != ''">
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
        <xsl:with-param name="RepetedRow">
          <xsl:value-of select="$RepetedRow"/>
        </xsl:with-param>
        <xsl:with-param name="TableColumnTagNum" select="$TableColumnTagNum"/>
        <xsl:with-param name="MergeCell" select="$MergeCell"/>
        <xsl:with-param name="tableName" select="$tableName"/>
      </xsl:apply-templates>
    </xsl:if>

  </xsl:template>

  <!-- Insert Conditional in Cell -->

  <xsl:template name="InsertConditionalCol">
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    <xsl:param name="TableColumnTagNum"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="tableName"/>
    <xsl:param name="RepetedRow"/>
    <xsl:param name="RepetedCol"/>

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

    <xsl:variable name="styleName">
      <xsl:choose>
        <xsl:when test="@table:style-name != '' ">
          <xsl:value-of select="@table:style-name"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$columnCellStyle"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="quot">
      <xsl:text>&quot;</xsl:text>
    </xsl:variable>

    <!-- if cell style has condition -->
    <!-- there was a case where condition was "cell-content()=#NAME?G$45" and it caused a crash (but there can be #NAME condition as text occurence) -->

<xsl:choose>
  <!-- Formulas are not implemented. We rejected conditional with formulas. -->
  <xsl:when test="key('style',$styleName)/style:map[@style:condition != '' and not(contains(@style:condition,'#NAME') and not(contains(@style:condition,$quot))) and not(contains(@style:condition, 'is-true-formula'))]">

 
      <xsl:variable name="ColChar">
        <xsl:call-template name="NumbersToChars">
          <xsl:with-param name="num">
            <xsl:value-of select="$colNumber"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>      
	 
      <xsl:variable name="ColCharEnd">
        <xsl:call-template name="NumbersToChars">
          <xsl:with-param name="num">
							<!--<xsl:choose>
              <xsl:when test="@table:number-columns-repeated != ''">
						  <xsl:choose >
							  <xsl:when test ="@table:number-columns-repeated &gt; 256">
								  <xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))- $RepetedCol + 1"/>
							  </xsl:when>
							  <xsl:when test ="@table:number-columns-repeated &lt;= 256">
                        <xsl:value-of select="$colNumber + @table:number-columns-repeated - $RepetedCol + 1"/>
              </xsl:when>
						  </xsl:choose>
					  </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$colNumber"/>
              </xsl:otherwise>
            </xsl:choose>-->
							<xsl:variable name ="tempOfficeVersion">
								<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
							</xsl:variable>
							<xsl:choose>
								<xsl:when test="@table:number-columns-repeated != ''">
									<xsl:choose>
										<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
											<xsl:value-of select="$colNumber + @table:number-columns-repeated - $RepetedCol + 1"/>
										</xsl:when>
										<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
											<xsl:choose>
												<xsl:when test ="@table:number-columns-repeated &gt; 256">
													<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))- $RepetedCol + 1"/>
												</xsl:when>
												<xsl:otherwise>
													<xsl:value-of select="$colNumber + @table:number-columns-repeated - $RepetedCol + 1"/>
												</xsl:otherwise>
											</xsl:choose>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="$colNumber + @table:number-columns-repeated - $RepetedCol + 1"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="$colNumber"/>
								</xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
	  <!--Vijayeta,SP2,@table:number-columns-repeated,End-->
    <oox:ConditionalCell>
      
      <xsl:attribute name="oox:StyleName">
        <xsl:value-of select="$styleName"/>
      </xsl:attribute>
      
      <xsl:choose>
      
      <xsl:when test="contains($MergeCell,concat($ColChar, $rowNumber))">
      <xsl:value-of
      select="concat($ColChar, $rowNumber,substring-before(substring-after($MergeCell,concat($ColChar, $rowNumber)),';'))"
      />
      </xsl:when>
      <xsl:otherwise>
      <xsl:choose>
      <xsl:when test="$RepetedRow != ''">
      <xsl:value-of select="concat($ColChar, $rowNumber, ':', $ColCharEnd, $rowNumber + $RepetedRow + 1)"/>
      </xsl:when>
<!--
    Code Change: Sonata
    Desc:        To avoid crash because of Condiitonal Formatting
    Defect :     1828899
 -->
      <!--<xsl:when test="$ColChar != $ColCharEnd">
      <xsl:value-of select="concat($ColChar, $rowNumber, ':', $ColCharEnd, $rowNumber)"/>
      </xsl:when>-->
      <xsl:otherwise>
      <!--<xsl:value-of select="concat($ColChar, $rowNumber)"/>-->
        <xsl:value-of select="concat($ColChar, $rowNumber, ':', $ColCharEnd, $rowNumber)"/>
      </xsl:otherwise>
      </xsl:choose>
      </xsl:otherwise>
      </xsl:choose>
    </oox:ConditionalCell>
    
  </xsl:when>
  <xsl:otherwise>
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
    <xsl:if test="@table:number-columns-repeated!= '' and $tableNumberColumnsRepeated &gt; $RepetedCol">
      <xsl:call-template name="InsertConditionalCol">
        <xsl:with-param name="colNumber">
          <xsl:value-of select="$colNumber + 1"/>
        </xsl:with-param>
        <xsl:with-param name="rowNumber">
          <xsl:value-of select="$rowNumber"/>
        </xsl:with-param>
        <xsl:with-param name="TableColumnTagNum">
          <xsl:value-of select="$TableColumnTagNum"/>
        </xsl:with-param>
        <xsl:with-param name="MergeCell">
          <xsl:value-of select="$MergeCell"/>
        </xsl:with-param>
        <xsl:with-param name="tableName">
          <xsl:value-of select="$tableName"/>
        </xsl:with-param>
        <xsl:with-param name="RepetedRow">
          <xsl:value-of select="$RepetedRow"/>
        </xsl:with-param>
        <xsl:with-param name="RepetedCol">
          <xsl:value-of select="$RepetedCol + 1"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
    
  </xsl:otherwise>
</xsl:choose>

  </xsl:template>

  <!-- Coditional Format -->

  <xsl:template name="InsertConditionalFormat">
	  <dxfs count="3">
		  <dxf>
			  <font>
				  <condense val="0"/>
				  <extend val="0"/>
				  <color rgb="FF9C0006"/>
			  </font>
			  <fill>
				  <patternFill>
					  <bgColor rgb="FFFFC7CE"/>
				  </patternFill>
			  </fill>
		  </dxf>
		  <xsl:for-each
		 select="office:document-content/office:automatic-styles/style:style/style:map[@style:condition != '']">
			  <xsl:variable name="StyleApplyStyleName">
				  <xsl:value-of select="@style:apply-style-name"/>
			  </xsl:variable>
			  <dxf>
				  <xsl:for-each
					select="document('styles.xml')/office:document-styles/office:styles">
					  <xsl:for-each select="style:style[@style:name=$StyleApplyStyleName]">
						  <font>
							  <xsl:call-template name="InsertTextProperties">
								  <xsl:with-param name="mode">default</xsl:with-param>
							  </xsl:call-template>
						  </font>
					  </xsl:for-each>
					  <!-- style:table-cell-properties fo:background-color       -->

					  <xsl:for-each select="style:style[@style:name=$StyleApplyStyleName]">
						  <xsl:apply-templates select="style:table-cell-properties" mode="background-color">
							  <xsl:with-param name="Object">
								  <xsl:text>conditional</xsl:text>
							  </xsl:with-param>
						  </xsl:apply-templates>
						  <xsl:apply-templates select="style:table-cell-properties" mode="border"/>
					  </xsl:for-each>
				  </xsl:for-each>
			  </dxf>
		  </xsl:for-each>
	  </dxfs>
  </xsl:template>

  <!-- Insert Formula of Coditional -->

  <xsl:template name="InsertCoditionalFormula">
    <xsl:param name="tableName"/>

	<xsl:variable name="apos">
  		<xsl:text>&apos;</xsl:text>
	</xsl:variable>

    <xsl:choose>
      <xsl:when test="contains(@style:condition, '.$#REF!$#REF!')">
        <formula/>
      </xsl:when>

      <!-- Condition: Formula is -->
      <xsl:when test="contains(@style:condition, 'is-true-formula')">
        <formula/>
      </xsl:when>

      <!-- Condition: Cell content is less than or equal to-->
      <xsl:when test="contains(@style:condition, '&lt;=')">
        <formula>
          <xsl:if
            test="starts-with(substring-after(@style:condition, '&lt;='),'&quot;')
            or starts-with(substring-after(@style:condition, '&lt;='),'[')
            or number(substring-after(@style:condition, '&lt;='))">
            <xsl:call-template name="TranslateFormula">
              <xsl:with-param name="formula" select="substring-after(@style:condition,'&lt;=')"/>
              <xsl:with-param name="tableName" select="$tableName"/>
            </xsl:call-template>
          </xsl:if>
        </formula>
      </xsl:when>

      <!-- Condition: Cell content is less than -->
      <xsl:when test="contains(@style:condition, '&lt;')">
      <formula>
          <xsl:choose>
            <xsl:when test="starts-with(substring-after(@style:condition, '&lt;'),'&quot;')
              or starts-with(substring-after(@style:condition, '&lt;') ,'[')
              or number(substring-after(@style:condition, '&lt;'))">
              <xsl:call-template name="TranslateFormula">
                <xsl:with-param name="formula" select="substring-after(@style:condition,'&lt;')"/>
                <xsl:with-param name="tableName" select="$tableName"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:when test="substring-before(substring-after(@style:condition, '&lt;'), $apos) = '['">
              <xsl:call-template name="TranslateFormula">
                <xsl:with-param name="formula" select="substring-after(@style:condition,'&lt;')"/>
                <xsl:with-param name="tableName" select="$tableName"/>
              </xsl:call-template>  
            </xsl:when>
            <xsl:otherwise>              
              <xsl:value-of select="number(substring-after(@style:condition, '&lt;'))"/>
            </xsl:otherwise>
          </xsl:choose>
          
        </formula>
      </xsl:when>

      <!-- Condition: Cell content is greater than or equal to-->
      <xsl:when test="contains(@style:condition, '&gt;=')">
        <formula>
          <xsl:choose>
            <xsl:when test="starts-with(substring-after(@style:condition, '&gt;=') ,'&quot;')
              or starts-with(substring-after(@style:condition, '&gt;='),'[')
              or number(substring-after(@style:condition, '&gt;='))">
              <xsl:call-template name="TranslateFormula">
                <xsl:with-param name="formula" select="substring-after(@style:condition,'&gt;=')"/>
                <xsl:with-param name="tableName" select="$tableName"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:when test="substring-before(substring-after(@style:condition, '&gt;='), $apos) = '['">
              <xsl:call-template name="TranslateFormula">
                <xsl:with-param name="formula" select="substring-after(@style:condition,'&gt;=')"/>
                <xsl:with-param name="tableName" select="$tableName"/>
              </xsl:call-template>  
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="number(substring-after(@style:condition, '&gt;='))"/>
            </xsl:otherwise>
          </xsl:choose>
        </formula>
      </xsl:when>

      <!-- Condition: Cell content is greater than -->
      <xsl:when test="contains(@style:condition, '&gt;')">
        <formula>
          <xsl:choose>
            <xsl:when test="starts-with(substring-after(@style:condition, '&gt;'),'&quot;')
            or starts-with(substring-after(@style:condition, '&gt;'),'[') or starts-with(substring-after(@style:condition, '&gt;'),'$')
            or number(substring-after(@style:condition, '&gt;'))">
              <xsl:call-template name="TranslateFormula">
                <xsl:with-param name="formula" select="substring-after(@style:condition,'&gt;')"/>
                <xsl:with-param name="tableName" select="$tableName"/>
              </xsl:call-template>
          </xsl:when>
            <xsl:when test="substring-before(substring-after(@style:condition, '&gt;'), $apos) = '['">
              <xsl:call-template name="TranslateFormula">
                <xsl:with-param name="formula" select="substring-after(@style:condition,'&gt;')"/>
                <xsl:with-param name="tableName" select="$tableName"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:when test="starts-with(substring-after(@style:condition, '&gt;'),'&quot;')
            or starts-with(substring-after(@style:condition, '&gt;'),'[')
            or number(substring-after(@style:condition, '&gt;'))">
              <xsl:call-template name="TranslateFormula">
                <xsl:with-param name="formula" select="substring-after(@style:condition,'&gt;')"/>
                <xsl:with-param name="tableName" select="$tableName"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:when test="number(substring-after(@style:condition, '&gt;')) and  number(substring-after(@style:condition, '&gt;'))!= ''">
              <xsl:value-of select="number(substring-after(@style:condition, '&gt;'))"/>
            </xsl:when>
          </xsl:choose>
        </formula>
      </xsl:when>

      <!-- Condition: Cell content is not equal to -->
      <xsl:when test="contains(@style:condition, '!=')">
        <formula>
          <xsl:choose>
            <xsl:when
              test="starts-with(substring-after(@style:condition, '!='),'&quot;')
              or starts-with(substring-after(@style:condition, '!='),'[')
              or number(substring-after(@style:condition, '!=')) or contains(substring-after(@style:condition, '!='), '&quot;')">
              <xsl:call-template name="TranslateFormula">
                <xsl:with-param name="formula" select="substring-after(@style:condition,'!=')"/>
                <xsl:with-param name="tableName" select="$tableName"/>
              </xsl:call-template>
            </xsl:when>
          </xsl:choose>
        </formula>
      </xsl:when>

      <!-- Condition: Cell content is equal to -->
      <xsl:when test="contains(@style:condition, '=')">
        <formula>
         <xsl:choose>
           <xsl:when test="starts-with(substring-after(@style:condition, '='),'&quot;')
            or starts-with(substring-after(@style:condition, '='),'[')
            or number(substring-after(@style:condition, '='))">
            <xsl:call-template name="TranslateFormula">
              <xsl:with-param name="formula" select="substring-after(@style:condition,'=')"/>
              <xsl:with-param name="tableName" select="$tableName"/>
            </xsl:call-template>
           </xsl:when>
           <xsl:when test = "substring-before(substring-after(@style:condition, '='), '$') = '['">
             <xsl:call-template name="TranslateFormula">
               <xsl:with-param name="formula" select="substring-after(@style:condition,'=')"/>
               <xsl:with-param name="tableName" select="$tableName"/>
             </xsl:call-template>             
           </xsl:when>
           <xsl:when test = "substring-before(substring-after(@style:condition, '='), $apos) = '['">
             <xsl:call-template name="TranslateFormula">
               <xsl:with-param name="formula" select="substring-after(@style:condition,'=')"/>
               <xsl:with-param name="tableName" select="$tableName"/>
             </xsl:call-template>             
           </xsl:when>           
           <xsl:when test="substring-after(@style:condition, '=')">
             <xsl:value-of select="number(substring-after(@style:condition, '='))"/>
          </xsl:when>
         </xsl:choose>
        </formula>
      </xsl:when>

      <!-- Condition: Cell content is between -->
      <!-- Condition: Cell content is not between -->
      <xsl:when test="contains(@style:condition, 'between')">
        <xsl:choose>
          <!-- two delimiting values are date values -->
          <xsl:when test="contains(@style:condition, 'DATE')">
            <formula>
              <xsl:value-of
                select="translate(substring-before(substring-after(@style:condition, '('), ','),';',',')"
              />
            </formula>
            <formula>
              <xsl:value-of
                select="concat(translate(substring-before(substring-after(@style:condition,','), ')'), ';',','), ')')"
              />
            </formula>
          </xsl:when>
          <xsl:otherwise>
            <formula>
              <xsl:choose>
				  
                <xsl:when
                  test="starts-with(substring-after(@style:condition, 'between'),'&quot;')
                   or starts-with(substring-after(@style:condition, 'between'),'[')
            or number(substring-after(@style:condition, 'between'))">
                  <xsl:call-template name="TranslateFormula">
                    <xsl:with-param name="formula"
                      select="substring-before(substring-after(@style:condition,'('),',')"/>
                    <xsl:with-param name="tableName" select="$tableName"/>
                  </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
					<xsl:choose>
						<xsl:when test ="substring-after(translate(translate(substring-before(substring-after(@style:condition, '('), ','),'[]',''),'.','!'),'$')=''">
							<xsl:value-of select ="substring-before(substring-after(@style:condition, '('), ',')"/>
						</xsl:when>
						<xsl:otherwise>
                  <xsl:value-of
                           select="substring-after(translate(translate(substring-before(substring-after(@style:condition, '('), ','),'[]',''),'.','!'),'$')"/>
						</xsl:otherwise>
					</xsl:choose>                 
                </xsl:otherwise>
              </xsl:choose>
            </formula>
            <formula>
              <xsl:choose>
                <xsl:when
                  test="starts-with(substring-after(@style:condition, 'between'),'&quot;')
                or starts-with(substring-after(@style:condition, 'between'),'[')
                or number(substring-after(@style:condition, 'between'))">
                  <xsl:call-template name="TranslateFormula">
                    <xsl:with-param name="formula"
                      select="substring-before(substring-after(@style:condition,','),')')"/>
                    <xsl:with-param name="tableName" select="$tableName"/>
                  </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
					<xsl:choose>
						<xsl:when test ="substring-after(translate(translate(substring-before(substring-after(@style:condition, ','), ')'),'[]',''),'.','!'),'$')=''">
                  <xsl:value-of select="substring-before(substring-after(@style:condition, ','), ')')"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of
					   select="substring-after(translate(translate(substring-before(substring-after(@style:condition, ','), ')'),'[]',''),'.','!'),'$')"/>
						</xsl:otherwise>
					</xsl:choose>					
                </xsl:otherwise>
              </xsl:choose>
            </formula>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>

      <xsl:otherwise>
        <formula>
          <!--          <xsl:choose>
            <xsl:when test="contains(@style:condition, '[')">
              <xsl:value-of
                select="substring-after(substring-before(substring-after(@style:condition, '['), ']'), '.')"
              />
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="substring-after(@style:condition, '=')"/>
            </xsl:otherwise>
          </xsl:choose>-->
        </formula>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="TranslateFormula">
    <xsl:param name="formula"/>
    <xsl:param name="operator"/>
    <xsl:param name="tableName"/>

    <xsl:variable name="apos">
      <xsl:text>&apos;</xsl:text>
    </xsl:variable>

    <xsl:choose>
      <!-- when formula starts with &apos; -->
      <xsl:when test="starts-with($formula,$apos)"/>
      <!-- when formula contains a function (or i.e (2+5)*12 )-->
      <xsl:when test="contains($formula,'(')"/>

      <!-- if condition is given by the formula /is-true-formula(...)/-->
      <xsl:when test="$operator != ''">
        <xsl:variable name="left">
          <xsl:value-of select="substring-before($formula,$operator)"/>
        </xsl:variable>

        <xsl:variable name="right">
          <xsl:value-of select="substring-after($formula,$operator)"/>
        </xsl:variable>

        <xsl:variable name="translatedLeft">
          <xsl:call-template name="TranslateReferences">
            <xsl:with-param name="string" select="$left"/>
            <xsl:with-param name="tableName" select="$tableName"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:variable name="translatedRight">
          <xsl:call-template name="TranslateReferences">
            <xsl:with-param name="string" select="$right"/>
            <xsl:with-param name="tableName" select="$tableName"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:value-of select="concat($translatedLeft,$operator,$translatedRight)"/>
      </xsl:when>

      <!-- if condition is given by the cell value -->
      <xsl:otherwise>
        <xsl:call-template name="TranslateReferences">
          <xsl:with-param name="string" select="$formula"/>
          <xsl:with-param name="tableName" select="$tableName"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="TranslateReferences">
    <xsl:param name="string"/>
    <xsl:param name="tableName"/>

    <xsl:variable name="apos">
      <xsl:text>&apos;</xsl:text>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="contains($string,'[')">
        <xsl:variable name="reference">
          <xsl:value-of select="substring-before(substring-after($string,'['),']')"/>
        </xsl:variable>
        <xsl:choose>
          <!-- when reference is to a cell in the same sheet ($sheet_name.ref or .ref)-->
          <xsl:when
            test="substring-before($reference,'.') = concat('$',$tableName) or substring-before($reference,'.') = '' ">
            <xsl:call-template name="TranslateReferences">
              <xsl:with-param name="string">
                <xsl:value-of select="substring-before($string,'[')"/>
                <xsl:value-of select="substring-after($reference,'.')"/>
                <xsl:value-of select="substring-after($string,']')"/>
              </xsl:with-param>
              <xsl:with-param name="tableName" select="$tableName"/>
            </xsl:call-template>
          </xsl:when>

          <!-- when reference is to a cell in another sheet -->
          <xsl:when test="substring-before($reference,'.') != concat('$',$tableName)">
            <xsl:variable name="sheet">
              <xsl:choose>
                <xsl:when test="starts-with($reference,'$')">
                  <xsl:value-of select="substring-after(substring-before($reference,'.'),'$')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="substring-before($reference,'.')"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="refSheetNumber">
              <xsl:for-each
                select="/office:document-content/office:body/office:spreadsheet/table:table[@table:name = translate($sheet,$apos,'')]">
                <xsl:number count="table:table" level="single"/>
              </xsl:for-each>
            </xsl:variable>

            <xsl:variable name="checkedName">
              <xsl:for-each
                select="/office:document-content/office:body/office:spreadsheet/table:table[@table:name = translate($sheet,$apos,'')]">
                <xsl:call-template name="CheckSheetName">
                  <xsl:with-param name="sheetNumber">
                    <xsl:value-of select="$refSheetNumber"/>
                  </xsl:with-param>
                  <xsl:with-param name="name">
                    <xsl:value-of
                      select="substring(translate($sheet,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31)"
                    />
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:for-each>
				
            </xsl:variable>

            <!--            <xsl:value-of select="$refSheetNumber"/>
            <xsl:text>####</xsl:text>
            <xsl:value-of select="$checkedName"/>-->
            <!--<xsl:value-of select="$sheet"/>-->
            <xsl:call-template name="TranslateReferences">
              <xsl:with-param name="string">
                <xsl:value-of select="substring-before($string,'[')"/>
                <!-- if sheet name has space than write name in apostrophes -->
                <xsl:if test="contains($checkedName,' ')">
                  <xsl:text>&apos;</xsl:text>
                </xsl:if>
                <xsl:value-of select="$checkedName"/>
                <xsl:if test="contains($checkedName,' ')">
                  <xsl:text>&apos;</xsl:text>
                </xsl:if>
                <xsl:text>!</xsl:text>
                <xsl:value-of select="substring-after($reference,'.')"/>
                <xsl:value-of select="substring-after($string,']')"/>
              </xsl:with-param>
              <xsl:with-param name="tableName" select="$tableName"/>
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <xsl:otherwise>
        <xsl:value-of select="$string"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>
  
  <xsl:template name="InsertConditionalCellElement">
    <xsl:param name="tableName"/>
    <xsl:param name="ConditionalCellStyle"></xsl:param>    
   
    <xsl:variable name="styleName">
        <xsl:value-of select="substring-before($ConditionalCellStyle, '|')"/>
    </xsl:variable>
   
    <xsl:variable name="quot">
      <xsl:text>&quot;</xsl:text>
    </xsl:variable>    
    <!-- if cell style has condition -->
	  <xsl:variable name ="condStyleTblColDummy">
		  <xsl:value-of select ="./table:table-column[@table:default-cell-style-name=$styleName]/@table:style-name"/>
	  </xsl:variable>
    <!-- there was a case where condition was "cell-content()=#NAME?G$45" and it caused a crash (but there can be #NAME condition as text occurence) -->
    <xsl:if test="(table:table-row/table:table-cell/@table:style-name = $styleName) or 
			($condStyleTblColDummy!='')">
		<!--table:table-row/table:table-cell/@table:style-name='') and-->
    <xsl:choose>
		
      <!-- Formulas are not implemented. We rejected conditional with formulas. -->
      <xsl:when test="key('style',$styleName)/style:map[@style:condition != '' and not(contains(@style:condition,'#NAME') and not(contains(@style:condition,$quot))) and not(contains(@style:condition, 'is-true-formula'))]">
        
        <conditionalFormatting>
          <xsl:attribute name="sqref">  
            <xsl:value-of select="$styleName"/>
          </xsl:attribute>
          <xsl:for-each
            select="key('style', $styleName)/style:map[not(contains(@style:condition,'#NAME') and not(contains(@style:condition,$quot)))]">
            <xsl:if test="not(contains(@style:condition,'is-true-formula'))">
              <cfRule stopIfTrue="1" type="cellIs" priority="{position()}">
                <xsl:if test="contains(@style:condition,'is-true-formula')">
                  <xsl:attribute name="type">
                    <xsl:text>expression</xsl:text>
                  </xsl:attribute>
                </xsl:if>
                <xsl:attribute name="dxfId">
                  <!-- style:map can also be in number style so we are counting only having style:style as a parent -->
                  <xsl:value-of
                    select="count(preceding::style:map[name(parent::node()) = 'style:style']) + 1"/>
                </xsl:attribute>
                <xsl:if test="not(contains(@style:condition,'is-true-formula'))">
                  <xsl:attribute name="operator">
                    <xsl:choose>
                      <xsl:when test="contains(@style:condition, '&lt;=')">
                        <xsl:text>lessThanOrEqual</xsl:text>
                      </xsl:when>
                      <xsl:when test="contains(@style:condition, '&lt;')">
                        <xsl:text>lessThan</xsl:text>
                      </xsl:when>
                      <xsl:when test="contains(@style:condition, '&gt;=')">
                        <xsl:text>greaterThanOrEqual</xsl:text>
                      </xsl:when>
                      <xsl:when test="contains(@style:condition, '&gt;')">
                        <xsl:text>greaterThan</xsl:text>
                      </xsl:when>
                      <xsl:when test="contains(@style:condition, '!=')">
                        <xsl:text>notEqual</xsl:text>
                      </xsl:when>
                      <xsl:when test="contains(@style:condition, 'cell-content-is-between')">
                        <xsl:text>between</xsl:text>
                      </xsl:when>
                      <xsl:when test="contains(@style:condition, 'cell-content-is-not-between')">
                        <xsl:text>notBetween</xsl:text>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:text>equal</xsl:text>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:attribute>
                </xsl:if>
                <xsl:variable name="styleCondition">
                  <xsl:value-of select="@style:condition"/>
                </xsl:variable>
                <xsl:if test="contains($styleCondition, concat('cell-content()=', $quot))">
                  <xsl:attribute name="type">
                    <xsl:value-of select="'cellIs'"/>
                  </xsl:attribute>
                  <xsl:attribute name="text">
                    <xsl:value-of select="substring-after($styleCondition, '=')"/>
                  </xsl:attribute>
                  <xsl:attribute name="operator">
                    <xsl:value-of select="'equal'"/>
                  </xsl:attribute>
                </xsl:if>
                <xsl:if test="not(contains($styleCondition, concat('cell-content()=', $quot)))">
                  <xsl:call-template name="InsertCoditionalFormula">
                    <xsl:with-param name="tableName" select="$tableName"/>
                  </xsl:call-template>
                </xsl:if>
                <xsl:if test="contains($styleCondition, concat('cell-content()=', $quot))">
                  <formula>
                    <xsl:value-of select="substring-after($styleCondition, '=')"/>
                  </formula>
                </xsl:if>
              </cfRule>
            </xsl:if>
          </xsl:for-each>
        </conditionalFormatting>
      </xsl:when>
       
    </xsl:choose>
    </xsl:if>
    
    <xsl:if test="substring-after($ConditionalCellStyle, '|') != ''">
    <xsl:call-template name="InsertConditionalCellElement">
      <xsl:with-param name="tableName">
        <xsl:value-of select="$tableName"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellStyle">
        <xsl:value-of select="substring-after($ConditionalCellStyle, '|')"/>
      </xsl:with-param>
    </xsl:call-template>
    </xsl:if>
    
  </xsl:template>

</xsl:stylesheet>
