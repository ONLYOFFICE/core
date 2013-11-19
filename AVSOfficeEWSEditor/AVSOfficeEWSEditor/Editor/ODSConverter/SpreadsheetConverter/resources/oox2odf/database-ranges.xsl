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
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main" exclude-result-prefixes="e ">

	<!--<xsl:import href="common.xsl"/>-->

	<xsl:template name="InsertDatabaseRange">
		<xsl:param name="number"/>
		<xsl:param name="checkedName"/>

		<!-- filter -->
		<xsl:for-each select="e:autoFilter">
			<table:database-range>
				<xsl:call-template name="MatchFilter">
					<xsl:with-param name="number" select="$number"/>
					<xsl:with-param name="checkedName" select="$checkedName"/>
				</xsl:call-template>

				<!-- filter can contain sort -->
				<xsl:for-each select="e:sortState">
					<xsl:call-template name="InsertRowSort"/>
				</xsl:for-each>
			</table:database-range>
		</xsl:for-each>

		<!-- sort -->
		<xsl:for-each select="e:sortState">
			<table:database-range>
				<xsl:call-template name="InsertSort">
					<xsl:with-param name="number" select="$number"/>
					<xsl:with-param name="checkedName" select="$checkedName"/>
				</xsl:call-template>
			</table:database-range>
		</xsl:for-each>

	</xsl:template>

	<xsl:template name="MatchFilter">
		<xsl:param name="number"/>
		<xsl:param name="checkedName"/>

		<xsl:variable name="filtersNum">
			<xsl:value-of select="count(e:filterColumn/e:filters)"/>
		</xsl:variable>
		<xsl:variable name="customFiltersNum">
			<xsl:value-of select="count(e:filterColumn/e:customFilters)"/>
		</xsl:variable>
		<xsl:variable name="topFiltersNum">
			<xsl:value-of select="count(e:filterColumn/e:top10)"/>
		</xsl:variable>

		<xsl:choose>
			<!-- filter combination: column value multiple selection plus other condition is not allowed in Calc -->
			<xsl:when
			  test="e:filterColumn/e:filters/e:filter[position() = 2] and $filtersNum + $customFiltersNum + $topFiltersNum &gt; 1">
				<xsl:message terminate="no">translation.oox2odf.RemovedFilter</xsl:message>
			</xsl:when>

			<xsl:otherwise>
				<xsl:call-template name="InsertFilter">
					<xsl:with-param name="checkedName" select="$checkedName"/>
				</xsl:call-template>
			</xsl:otherwise>

		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertFilter">
		<xsl:param name="checkedName"/>

		<xsl:attribute name="table:display-filter-buttons">
			<xsl:text>true</xsl:text>
		</xsl:attribute>
    <!--added by chhavi for sp2-->
		<!--<xsl:attribute name="table:name">
			<xsl:value-of select="translate($checkedName,' ','_')"/>
		</xsl:attribute>-->
    <!--end here-->
    
		<xsl:attribute name="table:target-range-address">

			<!-- sheet_name -->
			<xsl:if test="contains($checkedName,' ')">
				<xsl:text>&apos;</xsl:text>
			</xsl:if>
			<xsl:value-of select="$checkedName"/>
			<xsl:if test="contains($checkedName,' ')">
				<xsl:text>&apos;</xsl:text>
			</xsl:if>

			<xsl:text>.</xsl:text>
			<xsl:value-of select="substring-before(@ref,':')"/>
			<xsl:text>:</xsl:text>

			<!-- sheet_name -->
			<xsl:if test="contains($checkedName,' ')">
				<xsl:text>&apos;</xsl:text>
			</xsl:if>
			<xsl:value-of select="$checkedName"/>
			<xsl:if test="contains($checkedName,' ')">
				<xsl:text>&apos;</xsl:text>
			</xsl:if>

			<xsl:text>.</xsl:text>
			<xsl:value-of select="substring-after(@ref,':')"/>
		</xsl:attribute>

		<xsl:variable name="filtersNum">
			<xsl:value-of select="count(e:filterColumn/e:filters)"/>
		</xsl:variable>
		<xsl:variable name="customFiltersNum">
			<xsl:value-of select="count(e:filterColumn/e:customFilters)"/>
		</xsl:variable>
		<xsl:variable name="topFiltersNum">
			<xsl:value-of select="count(e:filterColumn/e:top10)"/>
		</xsl:variable>

		<xsl:if test="child::node()">
			<!-- 
			Defect:1995696
			Date  :26th June '08
			By    :Shabeer
			Desc  :Sometimes in filter type 'autofilter' in XLSX, tags 'filterColumn' appears with out any additional filter condition, but for filter buttons
				   In which case, in ODF tag 'table:filter' gets inserted, because of which filters are not retained in roundtrip.
				   Hence additional check to take care that if no conditions appear in filter, 'table:filter' is not inserted.
		-->
			<xsl:variable name ="flag">
				<xsl:for-each select =" e:filterColumn">
					<!-- checking done to insert filter conditions, if there is any-->
					<xsl:choose>
						<xsl:when test="count(child::node()) != 0">
							<xsl:value-of select ="'true'"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select ="'false'"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:for-each>
			</xsl:variable>
			<xsl:if test ="contains($flag, 'true')">
				<table:filter>
					<xsl:choose>
						<xsl:when test="$filtersNum + $customFiltersNum + $topFiltersNum &gt; 1">
							<table:filter-and>
								<xsl:for-each select="e:filterColumn">
									<xsl:call-template name="InsertFilterConditions"/>
								</xsl:for-each>
							</table:filter-and>
						</xsl:when>
						<xsl:otherwise>
							<xsl:for-each select="e:filterColumn">
								<xsl:call-template name="InsertFilterConditions">
									<xsl:with-param name="singleColumn" select="1"/>
								</xsl:call-template>
							</xsl:for-each>
						</xsl:otherwise>
					</xsl:choose>
				</table:filter>
			</xsl:if>
		</xsl:if>
	</xsl:template>

	<xsl:template name="InsertFilterConditions">
		<xsl:param name="singleColumn"/>

		<xsl:variable name="field">
			<xsl:value-of select="@colId"/>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="e:top10">
				<xsl:for-each select="e:top10">
					<table:filter-condition table:operator="=">
						<xsl:attribute name="table:field-number">
							<xsl:value-of select="$field"/>
						</xsl:attribute>
						<xsl:attribute name="table:value">
							<xsl:value-of select="@val"/>
						</xsl:attribute>
						<xsl:attribute name="table:operator">
							<xsl:call-template name="TranslateFilterOperator"/>
						</xsl:attribute>
					</table:filter-condition>
				</xsl:for-each>
			</xsl:when>

			<xsl:when test="count(e:filters/e:filter) &gt; 1">
				<table:filter-or>
					<xsl:for-each select="e:filters/e:filter">
						<table:filter-condition table:operator="=">
							<xsl:attribute name="table:field-number">
								<xsl:value-of select="$field"/>
							</xsl:attribute>
							<xsl:attribute name="table:value">
								<xsl:value-of select="@val"/>
							</xsl:attribute>
						</table:filter-condition>
					</xsl:for-each>
				</table:filter-or>
			</xsl:when>

			<xsl:when test="e:filters/e:filter">
				<xsl:for-each select="e:filters/e:filter">
					<table:filter-condition table:operator="=">
						<xsl:attribute name="table:field-number">
							<xsl:value-of select="$field"/>
						</xsl:attribute>
						<xsl:attribute name="table:value">
							<xsl:value-of select="@val"/>
						</xsl:attribute>
					</table:filter-condition>
				</xsl:for-each>
			</xsl:when>

			<xsl:when test="$singleColumn = 1 and e:customFilters/@and = 1">
				<table:filter-and>
					<xsl:for-each select="e:customFilters/e:customFilter">
						<table:filter-condition>
							<xsl:attribute name="table:operator">
								<xsl:call-template name="TranslateFilterOperator"/>
							</xsl:attribute>
							<xsl:attribute name="table:field-number">
								<xsl:value-of select="$field"/>
							</xsl:attribute>
							<xsl:attribute name="table:value">
								<xsl:call-template name="TranslateFilterValue"/>
							</xsl:attribute>
						</table:filter-condition>
					</xsl:for-each>
				</table:filter-and>
			</xsl:when>

			<xsl:when test="e:customFilters/e:customFilter">
				<xsl:for-each select="e:customFilters/e:customFilter">
					<table:filter-condition>
						<xsl:attribute name="table:operator">
							<xsl:call-template name="TranslateFilterOperator"/>
						</xsl:attribute>
						<xsl:attribute name="table:field-number">
							<xsl:value-of select="$field"/>
						</xsl:attribute>
						<xsl:attribute name="table:value">
							<xsl:call-template name="TranslateFilterValue"/>
						</xsl:attribute>
					</table:filter-condition>
				</xsl:for-each>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="TranslateFilterOperator">

		<xsl:choose>
			<xsl:when test="substring(@val,1,1) = '*' or substring(@val,string-length(@val),1) = '*' ">
				<xsl:if test="@operator = 'notEqual' ">
					<xsl:text>!</xsl:text>
				</xsl:if>
				<xsl:text>match</xsl:text>
			</xsl:when>
			<xsl:when test="@operator = 'notEqual' ">
				<xsl:text>!=</xsl:text>
			</xsl:when>
			<xsl:when test="@operator = 'greaterThan' ">
				<xsl:text>&gt;</xsl:text>
			</xsl:when>
			<xsl:when test="@operator = 'greaterThanOrEqual' ">
				<xsl:text>&gt;=</xsl:text>
			</xsl:when>
			<xsl:when test="@operator = 'lessThan' ">
				<xsl:text>&lt;</xsl:text>
			</xsl:when>
			<xsl:when test="@operator = 'lessThanOrEqual' ">
				<xsl:text>&lt;=</xsl:text>
			</xsl:when>
			<xsl:when test="name() = 'top10' and @top = 0 and @percent = 1">
				<xsl:text>bottom percent</xsl:text>
			</xsl:when>
			<xsl:when test="name() = 'top10' and @percent = 1">
				<xsl:text>top percent</xsl:text>
			</xsl:when>
			<xsl:when test="name() = 'top10' and @top = 0">
				<xsl:text>bottom values</xsl:text>
			</xsl:when>
			<xsl:when test="name() = 'top10' ">
				<xsl:text>top values</xsl:text>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="TranslateFilterValue">
		<xsl:choose>
			<!-- contains -->
			<xsl:when test="substring(@val,1,1) = '*' and substring(@val,string-length(@val),1) = '*' ">
				<xsl:text>.</xsl:text>
				<xsl:value-of select="substring(@val,1,string-length(@val)-1)"/>
				<xsl:text>.*</xsl:text>
			</xsl:when>
			<!-- begins with -->
			<xsl:when test="substring(@val,string-length(@val),1) = '*' ">
				<xsl:text>^</xsl:text>
				<xsl:value-of select="substring(@val,1,string-length(@val)-1)"/>
				<xsl:text>.*</xsl:text>
			</xsl:when>
			<!-- ends with -->
			<xsl:when test="substring(@val,1,1) = '*' ">
				<xsl:text>.</xsl:text>
				<xsl:value-of select="@val"/>
				<xsl:text>$</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="@val"/>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

	<xsl:template name="InsertSort">
		<xsl:param name="number"/>
		<xsl:param name="checkedName"/>
<!--added by chhavi for sp2 -->
		<!--<xsl:attribute name="table:name">
			<xsl:value-of select="translate($checkedName,' ','_')"/>
		</xsl:attribute>-->
    <!--end here-->
    
		<!-- Code Changed by: Vijayeta 
     Bug number:1877156 
	 Date:29thJAN 
	 Desc: If the attribute is not a square ref, that is if it does nt containg ':', it lead to infinite loop. Hence a condition added to test it-->
		<xsl:variable name="colStart">
			<xsl:choose>
				<xsl:when test ="contains(@ref, ':')">
					<xsl:call-template name="GetColNum">
						<xsl:with-param name="cell" select="substring-before(@ref,':')"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="GetColNum">
						<xsl:with-param name="cell" select="@ref"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<!-- Code Changed by: Vijayeta 
     Bug number:1877156 
	 Date:29thJAN 
	 Desc: If the attribute is not a square ref, that is if it does nt containg ':', it lead to infinite loop. Hence a condition added to test it-->
		<xsl:variable name="rowStart">
			<xsl:choose>
				<xsl:when test ="contains(@ref, ':')">
					<xsl:call-template name="GetRowNum">
						<xsl:with-param name="cell" select="substring-before(@ref,':')"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="GetRowNum">
						<xsl:with-param name="cell" select="@ref"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
			<!--<xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell" select="substring-before(@ref,':')"/>
      </xsl:call-template>-->
		</xsl:variable>

		<xsl:attribute name="table:target-range-address">

			<!-- sheet_name -->
			<xsl:if test="contains($checkedName,' ')">
				<xsl:text>&apos;</xsl:text>
			</xsl:if>
			<xsl:value-of select="$checkedName"/>
			<xsl:if test="contains($checkedName,' ')">
				<xsl:text>&apos;</xsl:text>
			</xsl:if>
			<xsl:text>.</xsl:text>

			<!-- range start (if there is a row/column before add it as a header) -->
			<xsl:choose>
				<xsl:when test="@columnSort = 1">

					<xsl:variable name="colNum">
						<xsl:choose>
							<!-- when it's the column A -->
							<xsl:when test="$colStart = 1">
								<xsl:value-of select="$colStart"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$colStart - 1"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>

					<xsl:call-template name="NumbersToChars">
						<!-- it is 0 based so decrease by 1 -->
						<xsl:with-param name="num" select="$colNum - 1"/>
					</xsl:call-template>
					<xsl:value-of select="$rowStart"/>
				</xsl:when>

				<xsl:otherwise>

					<xsl:call-template name="NumbersToChars">
						<!-- it is 0 based so decrease by 1 -->
						<xsl:with-param name="num" select="$colStart - 1"/>
					</xsl:call-template>

					<xsl:choose>
						<!-- when it's the row 1 -->
						<xsl:when test="$rowStart = 1">
							<xsl:value-of select="$rowStart"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$rowStart - 1"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>

			<xsl:text>:</xsl:text>

			<!-- sheet_name -->
			<!-- 
			 Code Changed by: Vijayeta 
			 Bug number:1803593, performance 
			 Date:19th Jun '08 
			 Desc: If the attribute is not a square ref, that is if it does nt contain ':', it leads to infinite loop. 
			 Hence a condition added to test it(line 435)
			 InputFile: Uni_Chart_Privatization.xlsx(roundtrip)
			 -->
			<xsl:if test="contains($checkedName,' ')">
				<xsl:text>&apos;</xsl:text>
			</xsl:if>
			<xsl:value-of select="$checkedName"/>
			<xsl:if test="contains($checkedName,' ')">
				<xsl:text>&apos;</xsl:text>
			</xsl:if>
			<xsl:text>.</xsl:text>
			<!--<xsl:value-of select="substring-after(@ref,':')"/>-->
			<xsl:choose>
				<xsl:when test ="contains(@ref, ':')">
					<xsl:value-of select="substring-after(@ref,':')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:variable name ="column">
						<xsl:call-template name="NumbersToChars">
							<!-- it is 0 based so decrease by 1 -->
							<xsl:with-param name="num" select="$colStart - 1"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:value-of select="concat($column,$rowStart - 1)"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
		<xsl:if test="(@columnSort = 1 and $colStart = 1) or (not(@columnSort) and $rowStart = 1)">
			<xsl:attribute name="table:contains-header">
				<xsl:text>false</xsl:text>
			</xsl:attribute>
		</xsl:if>
		<xsl:choose>
			<xsl:when test="@columnSort = 1">
				<xsl:call-template name="InsertColumnSort"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="InsertRowSort"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="InsertRowSort">

		<table:sort>

			<xsl:if test="@caseSensitive = 1">
				<xsl:attribute name="table:case-sensitive">
					<xsl:text>true</xsl:text>
				</xsl:attribute>
			</xsl:if>
			<!-- Code Changed by: Vijayeta 
     Bug number:1877156 
	 Date:29thJAN 
	 Desc: If the attribute is not a square ref, that is if it does nt containg ':', it lead to infinite loop. Hence a condition added to test it-->
			<xsl:variable name="colStart">
				<xsl:choose>
					<xsl:when test ="contains(@ref, ':')">
						<xsl:call-template name="GetColNum">
							<xsl:with-param name="cell" select="substring-before(@ref,':')"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="GetColNum">
							<xsl:with-param name="cell" select="@ref"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
				<!--<xsl:call-template name="GetColNum">
          <xsl:with-param name="cell" select="substring-before(@ref,':')"/>
        </xsl:call-template>-->
			</xsl:variable>

			<xsl:for-each select="e:sortCondition[not(@customList)]">
        <!--added by chhavi to fix sp2 problam (sortby is not supported by ods)-->
        <xsl:if test ="not(@sortBy)">
          <!--end here-->
				<table:sort-by table:data-type="automatic">

					<xsl:variable name="colNum">
						<xsl:if test="contains(@ref, ':')">
							<xsl:call-template name="GetColNum">
								<xsl:with-param name="cell" select="substring-before(@ref,':')"/>
							</xsl:call-template>
						</xsl:if>
						<xsl:if test="not(contains(@ref, ':'))">
							<xsl:call-template name="GetColNum">
								<xsl:with-param name="cell" select="@ref"/>
							</xsl:call-template>
						</xsl:if>
					</xsl:variable>

					<xsl:attribute name="table:field-number">
						<xsl:value-of select="$colNum - $colStart"/>
					</xsl:attribute>

					<xsl:if test="@descending = 1">
						<xsl:attribute name="table:order">
							<xsl:text>descending</xsl:text>
						</xsl:attribute>
					</xsl:if>
				</table:sort-by>
        </xsl:if>
			</xsl:for-each>
		</table:sort>
	</xsl:template>

	<xsl:template name="InsertColumnSort">

		<xsl:attribute name="table:orientation">
			<xsl:text>column</xsl:text>
		</xsl:attribute>

		<table:sort>

			<xsl:if test="@caseSensitive = 1">
				<xsl:attribute name="table:case-sensitive">
					<xsl:text>true</xsl:text>
				</xsl:attribute>
			</xsl:if>
			<!-- Code Changed by: Vijayeta 
     Bug number:1877156 
	 Date:29thJAN 
	 Desc: If the attribute is not a square ref, that is if it does nt containg ':', it lead to infinite loop. Hence a condition added to test it-->
			<xsl:variable name="rowStart">
				<xsl:choose>
					<xsl:when test ="contains(@ref, ':')">
						<xsl:call-template name="GetRowNum">
							<xsl:with-param name="cell" select="substring-before(@ref,':')"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="GetRowNum">
							<xsl:with-param name="cell" select="@ref"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
				<!--<xsl:call-template name="GetRowNum">
          <xsl:with-param name="cell" select="substring-before(@ref,':')"/>
        </xsl:call-template>-->
			</xsl:variable>

			<xsl:for-each select="e:sortCondition[not(@customList)]">
				<table:sort-by table:data-type="automatic">
					<!-- Code Changed by: Vijayeta 
     Bug number:1877156 
	 Date:29thJAN 
	 Desc: If the attribute is not a square ref, that is if it does nt containg ':', it lead to infinite loop. Hence a condition added to test it-->
					<xsl:variable name="rowNum">
						<xsl:choose>
							<xsl:when test ="contains(@ref, ':')">
								<xsl:call-template name="GetRowNum">
									<xsl:with-param name="cell" select="substring-before(@ref,':')"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="GetRowNum">
									<xsl:with-param name="cell" select="@ref"/>
								</xsl:call-template>
							</xsl:otherwise>
						</xsl:choose>
						<!--<xsl:call-template name="GetRowNum">
              <xsl:with-param name="cell" select="substring-before(@ref,':')"/>
            </xsl:call-template>-->
					</xsl:variable>

					<xsl:attribute name="table:field-number">
						<xsl:value-of select="$rowNum - $rowStart"/>
					</xsl:attribute>

					<xsl:if test="@descending = 1">
						<xsl:attribute name="table:order">
							<xsl:text>descending</xsl:text>
						</xsl:attribute>
					</xsl:if>

				</table:sort-by>
			</xsl:for-each>
		</table:sort>
	</xsl:template>

</xsl:stylesheet>