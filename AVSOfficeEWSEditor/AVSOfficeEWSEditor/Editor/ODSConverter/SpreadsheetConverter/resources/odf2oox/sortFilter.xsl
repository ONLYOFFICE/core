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
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0">

  <!--<xsl:import href="common.xsl"/>-->

  <xsl:template name="MatchFilter">
    <!-- @Description: Checks if filter can be conversed and returnes true or false when parameter 'ignoreFilter' is not empty otherwise makes the conversion -->
    <!-- @Context: chart:chart -->

    <xsl:param name="tableName"/>
    <xsl:param name="ignoreFilter"/>

    <xsl:variable name="apos">
      <xsl:text>&apos;</xsl:text>
    </xsl:variable>
	  <!-- 
	  Defect Id       :1958534
	  Defect Fixed By :Vijayeta
	  Date            :12/5/08
	  Desc            :An ODS file should have only a single filter option, when more than one present in XML,
	                   first filter option should only be considered.
	  -->
    <xsl:for-each
		   select="parent::node()/table:database-ranges/table:database-range[(table:filter or @table:display-filter-buttons = 'true') and substring-before(translate(@table:target-range-address,$apos,''),'.') = $tableName and position()=1]">

      <xsl:variable name="andFieldNumber">
        <xsl:value-of
          select="table:filter/table:filter-and/table:filter-condition[1]/@table:field-number"/>
      </xsl:variable>

      <xsl:variable name="orFieldNumber">
        <xsl:value-of
          select="table:filter/table:filter-or/table:filter-condition[1]/@table:field-number"/>
      </xsl:variable>

      <!-- select filter combinations wchich will be converted -->
      <xsl:choose>

        <!-- single condition filter (first 'or' when filter has no conditions)-->
        <xsl:when
          test="(@table:display-filter-buttons = 'true' and not (child::node())) or count(table:filter/child::node()) = 1 and table:filter/table:filter-condition">
          <xsl:call-template name="SingleConditionFilter"/>
        </xsl:when>

        <!-- single column values selection fiter -->
        <xsl:when
          test="count(table:filter/child::node()) = 1 and table:filter/table:filter-or/table:filter-condition and not(table:filter/table:filter-or/table:filter-condition/@table:operator != '=') and
          not(table:filter/table:filter-or/table:filter-condition[@table:field-number != $orFieldNumber])">

          <xsl:call-template name="ValueSelectionFilter"/>
        </xsl:when>

        <!-- single AND filter -->
        <xsl:when
          test="count(table:filter/child::node()) = 1 and table:filter/table:filter-and/table:filter-condition">

          <xsl:choose>
            <!-- when there is top 10 condition then for this filed can not be another condition -->
            <xsl:when
              test="table:filter/table:filter-and/table:filter-condition[@table:operator = 'top values' or  @table:operator = 'bottom values' or @table:operator = 'top percent' or 
              @table:operator = 'bottom percent' ]">

              <xsl:variable name="ignore">
                <xsl:for-each
                  select="table:filter/table:filter-and/table:filter-condition[@table:operator = 'top values' or  @table:operator = 'bottom values' or @table:operator = 'top percent' or 
                  @table:operator = 'bottom percent' ]">

                  <xsl:variable name="fieldNumber">
                    <xsl:value-of select="@table:field-number"/>
                  </xsl:variable>
                  <xsl:variable name="conditionId">
                    <xsl:value-of select="generate-id(.)"/>
                  </xsl:variable>

                  <xsl:if
                    test="parent::node()/table:filter-condition[@table:field-number = $fieldNumber and generate-id(.) != $conditionId]">
                    <xsl:text>ignore</xsl:text>
                  </xsl:if>
                </xsl:for-each>
              </xsl:variable>

              <xsl:choose>
                <xsl:when test="$ignore = '' ">
                  <xsl:call-template name="MultiColumnAndFilter"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:if test="$ignoreFilter != '' ">
                    <xsl:text>true</xsl:text>
                  </xsl:if>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:when>

            <xsl:otherwise>
              <xsl:call-template name="MultiColumnAndFilter"/>
            </xsl:otherwise>
          </xsl:choose>

        </xsl:when>
        <xsl:otherwise>
          <xsl:if test="$ignoreFilter != '' ">
            <xsl:text>true</xsl:text>
          </xsl:if>
        </xsl:otherwise>
      </xsl:choose>

    </xsl:for-each>
  </xsl:template>

  <xsl:template name="SingleConditionFilter">

    <autoFilter>
      <xsl:attribute name="ref">
        <xsl:value-of
          select="substring-after(substring-before(@table:target-range-address,':'),'.')"/>
        <xsl:text>:</xsl:text>
        <xsl:value-of select="substring-after(substring-after(@table:target-range-address,':'),'.')"
        />
      </xsl:attribute>

      <xsl:for-each select="table:filter/table:filter-condition">
        <filterColumn>
          <xsl:attribute name="colId">
            <xsl:value-of select="@table:field-number"/>
          </xsl:attribute>

          <!-- choose filter type -->
          <xsl:choose>
            <xsl:when
              test="@table:operator = 'top values' or @table:operator = 'bottom values'  or @table:operator = 'top percent' or @table:operator = 'bottom percent' ">
              <top10>
                <xsl:choose>
                  <xsl:when test="@table:value &lt;= 500">
                    <xsl:call-template name="InsertFilterConditions"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:attribute name="val">
                      <xsl:text>500</xsl:text>
                    </xsl:attribute>
                  </xsl:otherwise>
                </xsl:choose>
              </top10>
            </xsl:when>
            <xsl:when test="@table:operator = '=' ">
              <filters>
                <filter>
                  <xsl:call-template name="InsertFilterConditions"/>
                </filter>
              </filters>
            </xsl:when>
            <xsl:otherwise>
              <customFilters>
                <customFilter>
                  <xsl:call-template name="InsertFilterConditions"/>
                </customFilter>
              </customFilters>
            </xsl:otherwise>
          </xsl:choose>

        </filterColumn>
      </xsl:for-each>

      <!-- when sort is applyed to filter -->
      <xsl:if test="table:sort">
        <xsl:call-template name="InsertRowSort"/>
      </xsl:if>
    </autoFilter>
  </xsl:template>

  <xsl:template name="ValueSelectionFilter">

    <autoFilter>
      <xsl:attribute name="ref">
        <xsl:value-of
          select="substring-after(substring-before(@table:target-range-address,':'),'.')"/>
        <xsl:text>:</xsl:text>
        <xsl:value-of select="substring-after(substring-after(@table:target-range-address,':'),'.')"
        />
      </xsl:attribute>

      <xsl:for-each select="table:filter/table:filter-or">
        <filterColumn>
          <xsl:attribute name="colId">
            <xsl:value-of select="table:filter-condition[1]/@table:field-number"/>
          </xsl:attribute>

          <filters>
            <xsl:for-each select="table:filter-condition">
              <filter>
                <xsl:call-template name="InsertFilterConditions"/>
              </filter>
            </xsl:for-each>
          </filters>
        </filterColumn>
      </xsl:for-each>

      <!-- when sort is applyed to filter -->
      <xsl:if test="table:sort">
        <xsl:call-template name="InsertRowSort"/>
      </xsl:if>

    </autoFilter>
  </xsl:template>

  <xsl:template name="MultiColumnAndFilter">
    <autoFilter>
      <xsl:attribute name="ref">
        <xsl:value-of
          select="substring-after(substring-before(@table:target-range-address,':'),'.')"/>
        <xsl:text>:</xsl:text>
        <xsl:value-of select="substring-after(substring-after(@table:target-range-address,':'),'.')"
        />
      </xsl:attribute>

      <xsl:for-each select="table:filter/table:filter-and">
        <xsl:apply-templates select="table:filter-condition[1]">
          <xsl:with-param name="fieldsConversed" select=" ',' "/>
        </xsl:apply-templates>
      </xsl:for-each>

      <!-- when sort is applyed to filter -->
      <xsl:if test="table:sort">
        <xsl:call-template name="InsertRowSort"/>
      </xsl:if>

    </autoFilter>
  </xsl:template>

  <xsl:template name="InsertFilterConditions">

    <xsl:attribute name="val">
      <xsl:choose>
        <!-- when condition is expressed by regular expression (usually used for parts of text occurence) -->
        <xsl:when test="@table:operator = 'match' or @table:operator = '!match'">
          <xsl:choose>
            <!-- (not) contains "x" condition [.*x.*] -->
            <xsl:when
              test="starts-with(@table:value,'.*') and substring(@table:value,string-length(@table:value)-1) = '.*' ">
              <xsl:text>*</xsl:text>
              <xsl:value-of select="substring(@table:value,3,string-length(@table:value)-4)"/>
              <xsl:text>*</xsl:text>
            </xsl:when>

            <!-- (not) begins with "x" condition [^x.*] -->
            <xsl:when
              test="starts-with(@table:value,'^') and substring(@table:value,string-length(@table:value)-1) = '.*' ">
              <xsl:value-of select="substring(@table:value,2,string-length(@table:value)-3)"/>
              <xsl:text>*</xsl:text>
            </xsl:when>

            <!-- (not) ends with "x" condition [.*x$] -->
            <xsl:when
              test="starts-with(@table:value,'.*') and substring(@table:value,string-length(@table:value)) = '$' ">
              <xsl:text>*</xsl:text>
              <xsl:value-of select="substring(@table:value,3,string-length(@table:value)-3)"/>
            </xsl:when>

            <xsl:otherwise>
              <xsl:value-of select="@table:value"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>

        <xsl:otherwise>
          <xsl:value-of select="@table:value"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>

    <!-- translate operator -->
    <xsl:choose>
      <xsl:when test="@table:operator = '!match' or @table:operator = '!=' ">
        <xsl:attribute name="operator">
          <xsl:text>notEqual</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="@table:operator = '&lt;' ">
        <xsl:attribute name="operator">
          <xsl:text>lessThan</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="@table:operator = '&gt;' ">
        <xsl:attribute name="operator">
          <xsl:text>greaterThan</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="@table:operator = '&lt;=' ">
        <xsl:attribute name="operator">
          <xsl:text>lessThanOrEqual</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="@table:operator = '&gt;=' ">
        <xsl:attribute name="operator">
          <xsl:text>greaterThanOrEqual</xsl:text>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>

    <!-- if bottom condition -->
    <xsl:if test="@table:operator = 'bottom values' or @table:operator = 'bottom percent' ">
      <xsl:attribute name="top">
        <xsl:text>0</xsl:text>
      </xsl:attribute>
    </xsl:if>

    <!-- if percent condition -->
    <xsl:if test="@table:operator = 'bottom percent' or @table:operator = 'top percent' ">
      <xsl:attribute name="percent">
        <xsl:text>1</xsl:text>
      </xsl:attribute>
    </xsl:if>

  </xsl:template>

  <xsl:template match="table:filter-condition">
    <xsl:param name="fieldsConversed"/>

    <xsl:choose>
      <xsl:when test="not(contains($fieldsConversed,concat(',',@table:field-number,',')))">
        <xsl:variable name="fieldNumber">
          <xsl:value-of select="@table:field-number"/>
        </xsl:variable>

        <filterColumn>
          <xsl:attribute name="colId">
            <xsl:value-of select="$fieldNumber"/>
          </xsl:attribute>

          <xsl:choose>
            <!-- when there is only one condition for this field -->
            <xsl:when
              test="not(following-sibling::table:filter-condition[@table:field-number = $fieldNumber])">

              <!-- choose filter type -->
              <xsl:choose>
                <xsl:when
                  test="@table:operator = 'top values' or @table:operator = 'bottom values'  or @table:operator = 'top percent' or @table:operator = 'bottom percent' ">
                  <top10>
                    <xsl:call-template name="InsertFilterConditions"/>
                  </top10>
                </xsl:when>
                <xsl:when test="@table:operator = '=' ">
                  <filters>
                    <filter>
                      <xsl:call-template name="InsertFilterConditions"/>
                    </filter>
                  </filters>
                </xsl:when>

                <xsl:otherwise>
                  <customFilters>
                    <customFilter>
                      <xsl:call-template name="InsertFilterConditions"/>
                    </customFilter>
                  </customFilters>
                </xsl:otherwise>
              </xsl:choose>

            </xsl:when>

            <!-- when this field has more than one "AND" condition-->
            <xsl:otherwise>
              <customFilters and="1">
                <xsl:for-each
                  select="parent::node()/table:filter-condition[@table:field-number = $fieldNumber]">
                  <customFilter>
                    <xsl:call-template name="InsertFilterConditions"/>
                  </customFilter>
                </xsl:for-each>
              </customFilters>
            </xsl:otherwise>
          </xsl:choose>
        </filterColumn>

        <xsl:apply-templates select="following-sibling::table:filter-condition[1]">
          <xsl:with-param name="fieldsConversed">
            <xsl:value-of select="concat($fieldsConversed,$fieldNumber,',')"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>

      <!-- when this field was conversed before -->
      <xsl:otherwise>
        <xsl:apply-templates select="following-sibling::table:filter-condition[1]">
          <xsl:with-param name="fieldsConversed" select="$fieldsConversed"/>
        </xsl:apply-templates>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="InsertSort">
    <xsl:param name="tableName"/>

    <xsl:variable name="apos">
      <xsl:text>&apos;</xsl:text>
    </xsl:variable>

    <xsl:for-each
      select="parent::node()/table:database-ranges/table:database-range[table:sort and not(table:filter) and substring-before(translate(@table:target-range-address,$apos,''),'.') = $tableName]">

      <xsl:choose>
        <xsl:when test="@table:orientation = 'column' ">
          <xsl:call-template name="InsertColumnSort"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="InsertRowSort"/>
        </xsl:otherwise>
      </xsl:choose>

    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertRowSort">
    <sortState>
      <xsl:if test="table:sort/@table:case-sensitive = 'true' ">
        <xsl:attribute name="caseSensitive">
          <xsl:text>1</xsl:text>
        </xsl:attribute>
      </xsl:if>
		<!--Vijayeta,SP2,Uni_Chart_Privatization.ods-->
        <!--
      <xsl:variable name="row1">
        <xsl:call-template name="GetRowNum">
          <xsl:with-param name="cell"
            select="substring-after(substring-before(@table:target-range-address,':'),'.')"/>
        </xsl:call-template>
      </xsl:variable>
		-->
		<xsl:variable name="row1">
		  <xsl:call-template name="GetRowNum">
			  <xsl:with-param name="cell">
				  <xsl:choose>
					  <xsl:when test ="contains(@table:target-range-address,':')">
						  <xsl:value-of select="substring-after(substring-before(@table:target-range-address,':'),'.')"/>
					  </xsl:when>
					  <xsl:otherwise>
						  <xsl:value-of select="substring-after(@table:target-range-address,'.')"/>
					  </xsl:otherwise>
				  </xsl:choose>
			  </xsl:with-param>
		  </xsl:call-template>		 
      </xsl:variable>		
      <!--<xsl:variable name="col1">-->
		<!-- substring-before rowNum in cell coordinates-->
		<!--<xsl:value-of
          select="substring-before(substring-after(substring-before(@table:target-range-address,':'),'.'),$row1)"/>-->
      <!--</xsl:variable>-->
      <xsl:variable name="col1">
        <!-- substring-before rowNum in cell coordinates-->
			<xsl:choose>
				<xsl:when test ="contains(@table:target-range-address,':')">
					<xsl:value-of select="substring-before(substring-after(substring-before(@table:target-range-address,':'),'.'),$row1)"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="substring-before(substring-after(@table:target-range-address,'.'),$row1)"/>
				</xsl:otherwise>
			</xsl:choose>
      </xsl:variable>
      <!--<xsl:variable name="row2">
        <xsl:call-template name="GetRowNum">
          <xsl:with-param name="cell"
            select="substring-after(substring-after(@table:target-range-address,':'),'.')"/>
        </xsl:call-template>
      </xsl:variable>-->
      <xsl:variable name="row2">
			<xsl:choose>
				<xsl:when test ="contains(@table:target-range-address,':')">
        <xsl:call-template name="GetRowNum">
          <xsl:with-param name="cell"
            select="substring-after(substring-after(@table:target-range-address,':'),'.')"/>
        </xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="''"/>
				</xsl:otherwise>
			</xsl:choose>
      </xsl:variable>
      <xsl:variable name="col2">
        <xsl:value-of
          select="substring-before(substring-after(substring-after(@table:target-range-address,':'),'.'),$row2)"
        />
      </xsl:variable>
		<!--Vijayeta,SP2,Uni_Chart_Privatization.ods,End-->
	 <!-- 
	 Code Changed by: Vijayeta 
     Bug number:1803593, performance 
	 Date:19th Jun '08 
	 Desc: In the following variable, according to the commented 'When' condition, if the attribute '@table:contains-header' is absent the condition is executed.
	       hence and additional condition to check if it is present in the first place
     InputFile: Uni_Chart_Privatization.xlsx(roundtrip)
	 -->
      <xsl:variable name="startRow">
        <xsl:choose>
			<!--<xsl:when test="@table:contains-header = 'false'">-->
			<xsl:when test="@table:contains-header = 'false' or not(@table:contains-header) ">
            <xsl:value-of select="$row1"/>
          </xsl:when>
          <!-- make range one row shorter-->
          <xsl:otherwise>
            <xsl:value-of select="$row1 + 1"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <!-- sort range -->
	  <!--Vijayeta,SP2,Uni_Chart_Privatization.ods-->
      <!--<xsl:attribute name="ref">
			<xsl:value-of select="concat($col1,$startRow,':', $col2, $row2)"/>-->
        <!--xsl:value-of select="concat(substring(substring-after(substring-after(@table:target-range-address,':'),'.'),0,2),number(substring(substring-after(substring-after(@table:target-range-address,':'),'.'),2))-1)"/-->
	  <!--</xsl:attribute>-->		
      <xsl:attribute name="ref">
			<xsl:choose>
				<xsl:when test ="$col2!='' and $row2!=''">
        <xsl:value-of select="concat($col1,$startRow,':', $col2, $row2)"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="concat($col1,$startRow)"/>
				</xsl:otherwise>
			</xsl:choose>			
        <!--xsl:value-of select="concat(substring(substring-after(substring-after(@table:target-range-address,':'),'.'),0,2),number(substring(substring-after(substring-after(@table:target-range-address,':'),'.'),2))-1)"/-->
      </xsl:attribute>
      <xsl:for-each
        select="table:sort/table:sort-by[position()=1 or (@table:field-number!=preceding-sibling::table:sort-by/@table:field-number) or (@table:data-type-number!=preceding-sibling::table:sort-by/@table:data-type)]">
        <sortCondition>
          <!-- field selection -->
          <!--<xsl:attribute name="ref">
            <xsl:variable name="colNum">
              <xsl:call-template name="GetAlphabeticPosition">
                <xsl:with-param name="literal" select="$col1"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="fieldCol">
              <xsl:call-template name="NumbersToChars">
                <xsl:with-param name="num" select="$colNum - 1  + @table:field-number"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select="concat($fieldCol,$startRow,':',$fieldCol,$row2)"/>
          </xsl:attribute>-->
          <xsl:attribute name="ref">
            <xsl:variable name="colNum">
              <xsl:call-template name="GetAlphabeticPosition">
                <xsl:with-param name="literal" select="$col1"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="fieldCol">
              <xsl:call-template name="NumbersToChars">
                <xsl:with-param name="num" select="$colNum - 1  + @table:field-number"/>
              </xsl:call-template>
            </xsl:variable>
				<xsl:choose>
					<xsl:when test ="$col2!='' and $row2!=''">
            <xsl:value-of select="concat($fieldCol,$startRow,':',$fieldCol,$row2)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="concat($fieldCol,$startRow)"/>
					</xsl:otherwise>
				</xsl:choose>				
          </xsl:attribute>
			<!--Vijayeta,SP2,Uni_Chart_Privatization.ods,End-->
          <!-- descending order -->
          <xsl:if test="@table:order = 'descending' ">
            <xsl:attribute name="descending">
              <xsl:text>1</xsl:text>
            </xsl:attribute>
          </xsl:if>
        </sortCondition>
      </xsl:for-each>
    </sortState>
  </xsl:template>

  <xsl:template name="InsertColumnSort">
    <sortState columnSort="1">
      <xsl:if test="table:sort/@table:case-sensitive = 'true' ">
        <xsl:attribute name="caseSensitive">
          <xsl:text>1</xsl:text>
        </xsl:attribute>
      </xsl:if>
	  <!--Vijayeta,SP2,Uni_Chart_Privatization.ods-->
      <!--<xsl:variable name="row1">
        <xsl:call-template name="GetRowNum">
          <xsl:with-param name="cell"
            select="substring-after(substring-before(@table:target-range-address,':'),'.')"/>
        </xsl:call-template>
      </xsl:variable>-->
		<xsl:variable name="row1">
			<xsl:call-template name="GetRowNum">
				<xsl:with-param name="cell">
					<xsl:choose>
						<xsl:when test ="contains(@table:target-range-address,':')">
							<xsl:value-of select="substring-after(substring-before(@table:target-range-address,':'),'.')"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="substring-after(@table:target-range-address,'.')"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:with-param>
			</xsl:call-template>
      </xsl:variable>
      <!--<xsl:variable name="col1">-->
        <!-- substring-before rowNum in cell coordinates-->
		<!--<xsl:value-of
          select="substring-before(substring-after(substring-before(@table:target-range-address,':'),'.'),$row1)"
        />
      </xsl:variable>-->
		<xsl:variable name="col1">
			<!-- substring-before rowNum in cell coordinates-->
			<xsl:choose>
				<xsl:when test ="contains(@table:target-range-address,':')">
					<xsl:value-of select="substring-before(substring-after(substring-before(@table:target-range-address,':'),'.'),$row1)"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="substring-before(substring-after(@table:target-range-address,'.'),$row1)"/>
				</xsl:otherwise>
			</xsl:choose>
      </xsl:variable>
      <!--<xsl:variable name="row2">
        <xsl:call-template name="GetRowNum">
          <xsl:with-param name="cell"
            select="substring-after(substring-after(@table:target-range-address,':'),'.')"/>
        </xsl:call-template>
      </xsl:variable>-->
      <xsl:variable name="row2">
			<xsl:choose>
				<xsl:when test ="contains(@table:target-range-address,':')">
        <xsl:call-template name="GetRowNum">
          <xsl:with-param name="cell"
            select="substring-after(substring-after(@table:target-range-address,':'),'.')"/>
        </xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="''"/>
				</xsl:otherwise>
			</xsl:choose>
      </xsl:variable>
      <xsl:variable name="col2">
        <xsl:value-of
          select="substring-before(substring-after(substring-after(@table:target-range-address,':'),'.'),$row2)"
        />
      </xsl:variable>
		<!--Vijayeta,SP2,Uni_Chart_Privatization.ods,End-->
	 <!-- 
	 Code Changed by: Vijayeta 
     Bug number:1803593, performance 
	 Date:19th Jun '08 
	 Desc: In the following variable, according to the commented 'When' condition, if the attribute '@table:contains-header' is absent the condition is executed.
	       hence and additional condition to check if it is present in the first place.
     InputFile: Uni_Chart_Privatization.xlsx(roundtrip)
	 -->
      <xsl:variable name="startCol">
        <xsl:choose>
			<!--<xsl:when test="@table:contains-header = 'false'">-->
			<xsl:when test="@table:contains-header = 'false' or not(@table:contains-header) ">
            <xsl:value-of select="$col1"/>
          </xsl:when>
          <!-- make range one row shorter-->
          <xsl:otherwise>
            <xsl:variable name="colNum">
              <!-- A equals 0 -->
              <xsl:call-template name="GetAlphabeticPosition">
                <xsl:with-param name="literal" select="$col1"/>
              </xsl:call-template>
            </xsl:variable>

            <!-- A equals 1 so it's not necessary to increase by 1 -->
            <xsl:call-template name="NumbersToChars">
              <xsl:with-param name="num" select="$colNum"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <!-- sort range -->
	  <!--Vijayeta,SP2,Uni_Chart_Privatization.ods-->
      <!--<xsl:attribute name="ref">
        <xsl:value-of select="concat($startCol,$row1,':', $col2, $row2)"/>       
      </xsl:attribute>-->
      <xsl:attribute name="ref">
			<xsl:choose>
				<xsl:when test ="$col2!='' and $row2!=''">
        <xsl:value-of select="concat($startCol,$row1,':', $col2, $row2)"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="concat($startCol,$row1)"/>
				</xsl:otherwise>
			</xsl:choose>
        <!--xsl:value-of select="concat(substring(substring-after(substring-after(@table:target-range-address,':'),'.'),0,2),number(substring(substring-after(substring-after(@table:target-range-address,':'),'.'),2))-1)"/-->
      </xsl:attribute>
      <xsl:for-each
        select="table:sort/table:sort-by[position()=1 or (@table:field-number!=preceding-sibling::table:sort-by/@table:field-number) or (@table:data-type-number!=preceding-sibling::table:sort-by/@table:data-type)]">
        <sortCondition>
          <!-- field selection -->
          <!--<xsl:attribute name="ref">
            <xsl:value-of
              select="concat($startCol,$row1 + @table:field-number,':',$col2,$row1 + @table:field-number)"
            />
          </xsl:attribute>-->
			<xsl:attribute name="ref">
				<xsl:choose>
					<xsl:when test ="$col2!=''">
						<xsl:value-of select="concat($startCol,$row1 + @table:field-number,':',$col2,$row1 + @table:field-number)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="concat($startCol,$row1 + @table:field-number)"/>
					</xsl:otherwise>
				</xsl:choose>				
          </xsl:attribute>
			<!--Vijayeta,SP2,Uni_Chart_Privatization.ods,End-->
          <!-- descending order -->
          <xsl:if test="@table:order = 'descending' ">
            <xsl:attribute name="descending">
              <xsl:text>1</xsl:text>
            </xsl:attribute>
          </xsl:if>
        </sortCondition>
      </xsl:for-each>
    </sortState>
  </xsl:template>

</xsl:stylesheet>
